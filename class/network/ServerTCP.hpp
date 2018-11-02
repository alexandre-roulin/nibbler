#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/thread.hpp>
#include <nibbler.hpp>
#include <Univers.hpp>

#define BUFFER_LEN 1024
#define PORT "4242"

using boost::asio::ip::tcp;

class ServerTCP;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
private:
	tcp::socket socket_;
	boost::asio::streambuf streambuf_;
	bool isReady_;
	ServerTCP &serverTCP_;

	TCPConnection(boost::asio::io_service &io_service, ServerTCP &serverTCP);

	void handler_read(const boost::system::error_code &, size_t);

	void handle_write(const boost::system::error_code &, size_t);
public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	static pointer
	create(boost::asio::io_service &io_service, ServerTCP &serverTCP);

	tcp::socket &socket();

	void async_write(std::string message);

	void async_read();
};


class ServerTCP {
public:
	ServerTCP(Univers &univers, boost::asio::io_service &io_service);

	void async_write(std::string message);

	void remove(TCPConnection::pointer);
private:
	std::vector<TCPConnection::pointer> pointers;

	void start_accept();
	Univers &univers;
	boost::thread thread_accept;
	boost::asio::deadline_timer timer_accept;
	tcp::acceptor acceptor_;
};