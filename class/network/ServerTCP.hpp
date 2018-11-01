#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#define PORT "4242"
using boost::asio::ip::tcp;

class TCPConnection
		: public boost::enable_shared_from_this<TCPConnection> {
public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	static pointer create(boost::asio::io_service &io_service) {
		return pointer(new TCPConnection(io_service));
	}

	tcp::socket &socket() {
		return socket_;
	}

	void start() {
		for (;;) {
			std::string message_;
			std::cout << "Message to deliver : ";
			std::getline(std::cin, message_);
			boost::asio::async_write(socket_, boost::asio::buffer(message_),
									 boost::bind(&TCPConnection::handle_write,
												 shared_from_this(),
												 boost::asio::placeholders::error,
												 boost::asio::placeholders::bytes_transferred));
		}

	}

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
	ServerTCP(boost::asio::io_service &io_service) :
			acceptor_(io_service, tcp::endpoint(tcp::v4(), 4242)) {
		start_accept();
	}

private:
	void start_accept() {
		TCPConnection::pointer new_connection =
				TCPConnection::create(acceptor_.get_io_service());

		acceptor_.async_accept(new_connection->socket(),
							   boost::bind(&ServerTCP::handle_accept, this,
										   new_connection,
										   boost::asio::placeholders::error));
	}

	void handle_accept(TCPConnection::pointer new_connection,
					   const boost::system::error_code &error) {
		if (!error) {
			new_connection->start();
		}

		start_accept();
	}

	tcp::acceptor acceptor_;
};