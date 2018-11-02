#include <cstdlib>
#include <deque>
#include <iostream>
#include "asio.hpp"
#include "ClientTCP.hpp"

ClientTCP::ClientTCP(boost::asio::io_service &io, std::string &hostname)
		: resolver(io),
		  query(hostname, "4242"),
		  socket(io),
		  timer(io, boost::posix_time::seconds(1)) {
	std::cout << "Trying to connect" << std::endl;
	tcp::resolver::iterator it = resolver.resolve(query);
	boost::asio::connect(socket, it);
}

void ClientTCP::read_socket() {
	boost::asio::async_read_until(socket, b, '\n',
								  boost::bind(&ClientTCP::handle_read,
											  shared_from_this(),
											  boost::asio::placeholders::error,
											  boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::write_socket(std::string message) {
	boost::asio::async_write(socket, boost::asio::buffer(message),
							 boost::bind(&ClientTCP::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::handle_read(
		boost::system::error_code const &error_code, size_t len) {
	if (error_code == 0) {
		std::cout << static_cast<char const *>(b.data().data());
	}
	b.consume(len);
	read_socket();
}

void ClientTCP::handle_write(const boost::system::error_code &error_code, size_t len) {
}

ClientTCP::pointer_client
ClientTCP::create(boost::asio::io_service &io, std::string hostname) {
	return pointer_client(new ClientTCP(io, hostname));
}
