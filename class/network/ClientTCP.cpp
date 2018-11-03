#include <cstdlib>
#include <deque>
#include <iostream>
#include <nibbler.hpp>
#include "asio.hpp"
#include "ClientTCP.hpp"
#include <gui/Core.hpp>

ClientTCP::ClientTCP(Univers &univers, boost::asio::io_service &io,
					 std::string &hostname)
		: id_(0),
		  univers(univers),
		  resolver(io),
		  query(hostname, "4242"),
		  socket(io),
		  timer(io, boost::posix_time::seconds(1)) {
}

void ClientTCP::change_state_ready() {
	snakes[id_].isReady = !snakes[id_].isReady;
	std::string buffer;
	add_prefix(SNAKE, buffer, &snakes[id_], sizeof(Snake));
	write_socket(buffer);
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

void ClientTCP::write_socket(void const *data, size_t len) {
	boost::asio::async_write(socket, boost::asio::buffer(data, len),
							 boost::bind(&ClientTCP::handle_write,
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


void ClientTCP::parse_input(void const *input, size_t len) {

	eHeader header;
	size_t header_len = sizeof(eHeader);
	std::memcpy(&header, input, header_len);
	char *data_deserialize = new char[len - header_len - 1];
	std::cout << "H:" <<header << "Len : " << len <<  std::endl;

	std::memcpy(data_deserialize,
				reinterpret_cast<char const *>(input) + header_len,
				len - header_len - 1);

	std::cout << "POST:H:" <<header << "CHAR_MAX :" << std::to_string(data_deserialize[len - header_len - 1])<< std::endl;
	switch (header) {
		case CHAT:
			univers.getCore_().addMessageChat(std::string(data_deserialize));
			break;
		case SNAKE_ARRAY: {
			std::cout << "Assert mano : "<<(len - header == sizeof(Snake) * MAX_SNAKE) << std::endl;
			std::memcpy(snakes, data_deserialize, sizeof(Snake) * MAX_SNAKE);
			break;
		}
		case SNAKE: {
			Snake snake_temp = *reinterpret_cast<Snake *>(data_deserialize);
			snakes[snake_temp.id] = snake_temp;
			break;
		}
		case FOOD:
			break;
		case ID:
			id_ = *reinterpret_cast<int16_t const *>(data_deserialize);
			std::cout << id_ << std::endl;
			break;
	}
	delete [] data_deserialize;
}

void ClientTCP::handle_read(
		boost::system::error_code const &error_code, size_t len) {
	if (error_code.value() == 0 && len > 0) {
//		std::cout << std::to_string(reinterpret_cast<char const *>(streambuf_.data().data())[len - 1]) << std::endl;
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

Snake const *ClientTCP::getSnakes() const {
	return snakes;
}
