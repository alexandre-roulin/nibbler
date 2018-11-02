#include <cstdlib>
#include <deque>
#include <iostream>
#include <nibbler.hpp>
#include "asio.hpp"
#include "ClientTCP.hpp"
#include <gui/Core.hpp>

ClientTCP::ClientTCP(Univers &univers, boost::asio::io_service &io,
					 std::string &hostname)
		: univers(univers),
		  resolver(io),
		  query(hostname, "4242"),
		  socket(io),
		  timer(io, boost::posix_time::seconds(1)) {
}

void ClientTCP::connect() {
	std::cout << "Trying to connect" << std::endl;
	tcp::resolver::iterator it = resolver.resolve(query);
	boost::asio::connect(socket, it);
}

void ClientTCP::read_socket() {
	boost::asio::async_read_until(socket, streambuf_, '\n',
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

void ClientTCP::add_prefix(eHeader header, std::string &message) {
	char *header_serialized = reinterpret_cast<char *>(&header);
	message.insert(0, header_serialized, sizeof(eHeader));
}

void ClientTCP::parse_input(void const *input, size_t len) {
	eHeader header;
	size_t header_len = sizeof(eHeader);
	std::memcpy(&header, input, header_len);
	switch (header) {
		case CHAT: {
			this->univers.getCore_().addMessageChat(
				reinterpret_cast<char const *> (streambuf_.data().data()) + header_len);
			break;
		}
		case SNAKE:
			break;
		case FOOD:
			break;
	}
}

void ClientTCP::handle_read(
		boost::system::error_code const &error_code, size_t len) {
	if (error_code.value() == 0 && len > 0) {
		parse_input(streambuf_.data().data(), len);
	}
	streambuf_.consume(len);
	read_socket();
}

void ClientTCP::handle_write(const boost::system::error_code &error_code,
							 size_t len) {
}

ClientTCP::pointer_client
ClientTCP::create(Univers &univers, boost::asio::io_service &io,
				  std::string hostname) {
	return pointer_client(new ClientTCP(univers, io, hostname));
}
