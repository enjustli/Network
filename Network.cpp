#pragma once

#include "Network.h"
#include <thread>
using namespace boost::asio;

namespace NetWork
{

TcpServer::TcpServer(std::function<void((boost::shared_ptr<boost::asio::ip::tcp::socket> psocket))> _handler,
                     std::string ipadress, int serverport)
    : acceptor(io_context, ip::tcp::endpoint(ip::address::from_string(ipadress), serverport))
{
    handler = _handler;
}

TcpServer::~TcpServer()
{
    stop();
}

void TcpServer::Accept_handler(boost::shared_ptr<boost::asio::ip::tcp::socket> psocket, boost::system::error_code ec)
{
    if (ec)
    {
        //std::cerr << ec.message() << std::endl;
        throw ec;
        return;
    }
    //继续异步
    start();
    /* boost::system::error_code _re_ec;
    std::size_t byte_transfer =
        boost::asio::read(*psocket, boost::asio::buffer(buffer), boost::asio::transfer_all(), _re_ec);
    if (_re_ec)
    {
        std::cerr << _re_ec.message() << std::endl;
        return;
    } */
    //处理函数
    handler(psocket);
}

void TcpServer::start()
{
    boost::shared_ptr<ip::tcp::socket> pSocket(new ip::tcp::socket(io_context));
    acceptor.async_accept(*pSocket, boost::bind(&TcpServer::Accept_handler, this, pSocket, _1));
}

void TcpServer::run(bool block)
{
    boost::system::error_code ec;
    if (block)
        io_context.run(ec);
    else
    {
        std::thread asyc(io_context.run, ec);
        asyc.detach();
    }
    throw ec;
}

void TcpServer::stop()
{
    io_context.stop();
}

void TcpServer::send(char *buffer, std::size_t bytes_transferred,
                     boost::shared_ptr<boost::asio::ip::tcp::socket> psocket, bool block = false)
{
    //int flags;
    if (block)
    {
        boost::system::error_code ec;
        boost::asio::write(*psocket, boost::asio::buffer(buffer, bytes_transferred), ec);
        //psocket->send(boost::asio::buffer(buffer, bytes_transferred));
        throw ec;
    }
    else
    {
        //关闭回调。。。。
        boost::asio::async_write(*psocket, boost::asio::buffer(buffer, bytes_transferred), nullptr);
    }
}

void TcpServer::receive(char *buffer, std::size_t &bytes_transferred,
                        boost::shared_ptr<boost::asio::ip::tcp::socket> psocket, bool block = false)
{
    //int flags;
    if (block)
    {
        boost::system::error_code ec;
        bytes_transferred =
            boost::asio::read(*psocket, boost::asio::buffer(buffer, bytes_transferred), boost::asio::transfer_all(), ec);
        //psocket->send(boost::asio::buffer(buffer, bytes_transferred));
        throw ec;
    }
    else
    {
        //关闭回调。。。。
        boost::asio::async_read(*psocket,
                                boost::asio::buffer(buffer, bytes_transferred), boost::asio::transfer_all(), nullptr);
    }
}





TcpClient::TcpClient() : socket(io_context){};

//连接,dns resolver
void TcpClient::connect(std::string domain, std::string serv_name, bool block = true)
{
    ip::tcp::resolver resolver(io_context);
    ip::tcp::resolver::results_type endpoint = resolver.resolve(domain, serv_name);
    if (block)
    {
        boost::asio::connect(socket, endpoint);
    }
    else
    {
        //关闭回调。。。。
        boost::asio::async_connect(socket, endpoint, nullptr);
    }
}

//连接
void TcpClient::connect(std::string ipadress, int remoteport, bool block = true)
{
    ip::tcp::endpoint endpoint(ip::address::from_string(ipadress), remoteport);
    if (block)
    {
        boost::asio::connect(socket, endpoint);
    }
    else
    {
        //关闭回调。。。。
        boost::asio::async_connect(socket, endpoint, nullptr);
    }
}

//断开
void TcpClient::disconnect(void)
{
    socket.close();
}

//发送数据，默认阻塞
void TcpClient::send(char *buffer, std::size_t bytes_transferred, bool block = true)
{
    if (block)
    {
        boost::system::error_code ec;
        boost::asio::write(socket, boost::asio::buffer(buffer, bytes_transferred), ec);
        //psocket->send(boost::asio::buffer(buffer, bytes_transferred));
        throw ec;
    }
    else
    {
        //关闭回调。。。。
        boost::asio::async_write(socket, boost::asio::buffer(buffer, bytes_transferred), nullptr);
    }
}

//接收数据，默认阻塞
void TcpClient::receive(char *buffer, std::size_t &bytes_transferred, bool block = true)
{
    if (block)
    {
        boost::system::error_code ec;
        bytes_transferred =
            boost::asio::read(socket, boost::asio::buffer(buffer, bytes_transferred), boost::asio::transfer_all(), ec);
        //psocket->send(boost::asio::buffer(buffer, bytes_transferred));
        throw ec;
    }
    else
    {
        //关闭回调。。。。
        boost::asio::async_read(socket,
                                boost::asio::buffer(buffer, bytes_transferred), boost::asio::transfer_all(), nullptr);
    }
}

} // namespace NetWork
