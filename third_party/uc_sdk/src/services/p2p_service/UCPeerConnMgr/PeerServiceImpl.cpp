#include "includes.h"
#include "PeerServiceImpl.h"

PeerServiceImpl::PeerServiceImpl(io_service &iosrv) : m_iosrv(iosrv), m_acceptor(iosrv, ip::tcp::endpoint(ip::tcp::v4(), 1000))
{
}


PeerServiceImpl::~PeerServiceImpl(void)
{
}

void PeerServiceImpl::start()
{
	LOGI("PeerServiceImpl::start");
	// 开始等待连接（非阻塞）
	boost::shared_ptr<ip::tcp::socket> psocket(new ip::tcp::socket(m_iosrv));

	// 触发的事件只有error_code参数，所以用boost::bind把socket绑定进去
	m_acceptor.async_accept(*psocket, boost::bind(&PeerServiceImpl::accept_handler, this, psocket, _1));
}

void PeerServiceImpl::accept_handler(boost::shared_ptr<ip::tcp::socket> psocket, error_code ec)
{
	if (ec) return;

	// 继续等待连接
	start();

	// 显示远程IP
	LOGI("remote_endpoint %s", psocket->remote_endpoint().address());

	// 发送信息(非阻塞)
	boost::shared_ptr<std::string> pstr(new std::string("hello async world!"));
	psocket->async_write_some(buffer(*pstr),
		boost::bind(&PeerServiceImpl::write_handler, this, pstr, _1, _2)
		);
}

void PeerServiceImpl::write_handler(boost::shared_ptr<std::string> pstr, error_code ec, size_t bytes_transferred)
{
	if (ec)
		LOGI("发送失败!");
	else
		LOGI("%s 已发送", *pstr);
}