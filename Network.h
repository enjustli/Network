

//一个利用boost asio 1.67构建的便于快速开发的network IO库

#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace NetWork
{

//TCP SERVER,异步
class TcpServer
{
private:
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::acceptor acceptor;

private:
  //任务处理函数
  std::function<void((boost::shared_ptr<boost::asio::ip::tcp::socket> psocket))> handler;
  //接受处理函数
  void Accept_handler(boost::shared_ptr<boost::asio::ip::tcp::socket> psocket, boost::system::error_code ec);

public:
  TcpServer(std::function<void((boost::shared_ptr<boost::asio::ip::tcp::socket> psocket))> _handler,
            std::string ipadress = "127.0.0.1", int serverport = 0);
  ~TcpServer();
  void start();
  //选择是否阻塞当前线程,默认不阻塞
  void run(bool block = false);
  void stop();
  //用于handler使用，发送数据
  void send(char *buffer, std::size_t bytes_transferred,
            boost::shared_ptr<boost::asio::ip::tcp::socket> psocket, bool block = true);
  //用于handler使用，接收数据
  void receive(char *buffer, std::size_t &bytes_transferred,
               boost::shared_ptr<boost::asio::ip::tcp::socket> psocket, bool block = true);
};

//TCP CLIENT,异步或者同步
class TcpClient
{
private:
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::socket socket;
  //boost::asio::ip::tcp::endpoint remote_endpoint;

public:
  TcpClient();
  //连接,dns resolver
  void connect(std::string domain, std::string serv_name, bool block = true);
  //连接
  void connect(std::string ipadress, int remoteport, bool block = true);
  //断开
  void disconnect(void);
  //发送数据，默认阻塞
  void send(char *buffer, std::size_t bytes_transferred, bool block = true);
  //接收数据，默认阻塞
  void receive(char *buffer, std::size_t &bytes_transferred, bool block = true);
};

//UDP PEER,异步或者同步
class UdpPeer
{
private:
  boost::asio::io_context io_context;
  boost::asio::ip::udp::socket socket;

public:
  UdpPeer();
  //发送数据，默认阻塞
  void send(char *buffer, std::size_t bytes_transferred, bool block = true);
  //接收数据，默认阻塞
  void receive(char *buffer, std::size_t &bytes_transferred, bool block = true);
};

} // namespace NetWork