#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

using namespace boost::asio;
using boost::system::error_code;

class PeerServiceImpl
{
public:
	PeerServiceImpl(io_service &iosrv);
	~PeerServiceImpl(void);

	//创建一个tcp的socket；且还是侦听
	void start();

	// 有客户端连接时accept_handler触发
	void accept_handler(boost::shared_ptr<ip::tcp::socket> psocket, error_code ec);

	// 异步写操作完成后write_handler触发
	void write_handler(boost::shared_ptr<std::string> pstr, error_code ec, size_t bytes_transferred);

private:
	io_service &m_iosrv;
	ip::tcp::acceptor m_acceptor;
};

