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

#define PORT "4242"
using boost::asio::ip::tcp;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	static pointer create(boost::asio::io_service &io_service);

	tcp::socket &getSocket();

	void start();

private:
	TCPConnection(boost::asio::io_service &io_service)
			: socket_(io_service) {
	}

	void handle_write(const boost::system::error_code & /*error*/,
					  size_t /*bytes_transferred*/) {
	}

	tcp::socket socket_;
	std::string message_;
};


class ServerTCP {
public:
	ServerTCP(boost::asio::io_service &io_service);

	int size_pointers();
private:
	std::vector<TCPConnection::pointer> pointers;
	void start_accept();

	tcp::acceptor acceptor_;
};