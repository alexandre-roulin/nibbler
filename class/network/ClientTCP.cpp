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

void ClientTCP::read_socket_header() {

	std::cout << "ClientTCP::read_socket_header" << std::endl;
	boost::asio::async_read(socket, boost::asio::buffer(buffer_data, sizeof(eHeader)),
							boost::bind(&ClientTCP::handle_read_header,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::read_socket_data(eHeader header) {
	std::cout << "ClientTCP::read_socket_data" << std::endl;
	boost::asio::async_read(socket, boost::asio::buffer(buffer_data, header),
							boost::bind(&ClientTCP::handle_read_data,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::read_socket_chat() {
	buffer_chat.prepare(CHAT_BUFFER);
	std::cout << "ClientTCP::read_socket_chat" << std::endl;
	boost::asio::async_read_until(socket, buffer_chat, '\n',
								  boost::bind(&ClientTCP::handle_read_chat,
											  shared_from_this(),
											  boost::asio::placeholders::error,
											  boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::handle_read_header(const boost::system::error_code &error_code,
								   size_t len) {
	std::cout << "ClientTCP::handle_read_header > len : " << len  << std::endl;
	if (error_code.value() == 0) {
		assert(len == sizeof(eHeader));

		eHeader header;
		std::memcpy(&header, buffer_data.data(), sizeof(eHeader));

		std::cout << "Header : " << header << std::endl;
		switch (header) {
			case CHAT:
				read_socket_chat();
				break;
			case SNAKE_ARRAY:
				read_socket_data(SNAKE_ARRAY);
				break;
			case SNAKE:
				read_socket_data(SNAKE);
				break;
			case FOOD:
				break;
			case ID:
				read_socket_data(ID);
				break;
		}
	}
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


void ClientTCP::parse_input(eHeader header , void const *input, size_t len) {

	std::cout << "data_deserialize [" << len << "]" << std::endl;

	char *data_deserialize = new char[len];
//	for (int i = 0; i < len; ++i) {
//		std::cout << static_cast<int>(data_deserialize[i]) << std::endl;
//	}
	std::memcpy(data_deserialize, reinterpret_cast<char const *>(input), len);

	switch (header) {
		case CHAT:
			data_deserialize[len - 1] = '\0';
			univers.getCore_().addMessageChat(std::string(data_deserialize, len));
			break;
		case SNAKE_ARRAY: {
			std::cout << "parse_input::SNAKE_ARRAY" << std::endl;
			std::memcpy(snakes, data_deserialize, sizeof(Snake) * MAX_SNAKE);
			break;
		}
		case SNAKE: {
			std::cout << "parse_input::SNAKE" << std::endl;
			Snake snake_temp = *reinterpret_cast<Snake *>(data_deserialize);
			snakes[snake_temp.id] = snake_temp;
			break;
		}
		case FOOD:
			break;
		case ID:
			std::cout << "parse_input::ID" << std::endl;
			id_ = *reinterpret_cast<int16_t const *>(data_deserialize);
			std::cout << "New id : " << id_ << std::endl;
			break;
	}
	delete [] data_deserialize;
}

void ClientTCP::handle_read_data(boost::system::error_code const &error_code,
							size_t len) {
	std::cout << "ClientTCP::handle_read_data [" << len << "]" << std::endl;

	if (error_code.value() == 0 && len > 0) {
		eHeader header = static_cast<eHeader >(len);
		parse_input(header, buffer_data.data(), len);
	}
	buffer_chat.consume(len);
	read_socket_header();
}

void ClientTCP::handle_read_chat(boost::system::error_code const &error_code,
								 size_t len) {
	std::cout << "ClientTCP::handle_read_chat" << std::endl;

	if (error_code.value() == 0 && len > 0) {
		parse_input(CHAT, buffer_chat.data().data(), len);
	}
	buffer_chat.consume(len);
	read_socket_header();
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
