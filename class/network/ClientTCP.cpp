#include <cstdlib>
#include <deque>
#include <iostream>
#include <nibbler.hpp>
#include "asio.hpp"
#include "ClientTCP.hpp"
#include <gui/Core.hpp>

int const ClientTCP::size_header[] = {
		[CHAT] = CHAT_BUFFER,
		[FOOD] = sizeof(int) * 2,
		[ID] = sizeof(int16_t),
		[START_GAME] = sizeof(int16_t),
		[SNAKE] = sizeof(Snake),
		[SNAKE_ARRAY] = sizeof(Snake) * MAX_SNAKE,
		[HEADER] = sizeof(eHeader)
};

ClientTCP::ClientTCP(Univers &univers, boost::asio::io_service &io,
					 std::string &hostname)
		: id_(0),
		  univers(univers),
		  resolver(io),
		  query(hostname, "4242"),
		  socket(io),
		  timer(io, boost::posix_time::seconds(1)),
		  factory(univers.getWorld_()) {
}

void ClientTCP::change_state_ready() {
	snake_array[id_].isReady = !snake_array[id_].isReady;
	std::string buffer;
	add_prefix(SNAKE, buffer, &snake_array[id_], sizeof(Snake));
	write_socket(buffer);
}

void ClientTCP::connect() {
	std::cout << "Trying to connect" << std::endl;
	tcp::resolver::iterator it = resolver.resolve(query);
	boost::asio::connect(socket, it);
}

void ClientTCP::read_socket_header() {

	std::cout << "ClientTCP::read_socket_header" << std::endl;
	boost::asio::async_read(socket, boost::asio::buffer(buffer_data,
														ClientTCP::size_header[HEADER]),
							boost::bind(&ClientTCP::handle_read_header,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::read_socket_data(eHeader header) {
	std::cout << "ClientTCP::read_socket_data" << std::endl;
	boost::asio::async_read(socket, boost::asio::buffer(buffer_data,
														ClientTCP::size_header[header]),
							boost::bind(&ClientTCP::handle_read_data,
										shared_from_this(),
										header,
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::handle_read_header(const boost::system::error_code &error_code,
								   size_t len) {
	std::cout << "ClientTCP::handle_read_header > len : " << len << std::endl;
	if (error_code.value() == 0) {
		assert(len == sizeof(eHeader));
		eHeader header;
		std::memcpy(&header, buffer_data.data(), sizeof(eHeader));
		std::cout << "Header : " << header << std::endl;
		read_socket_data(header);
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


void ClientTCP::parse_input(eHeader header, void const *input, size_t len) {


	char *data_deserialize = new char[len];
	std::memcpy(data_deserialize, reinterpret_cast<char const *>(input), len);

	switch (header) {
		case CHAT:
			univers.getCore_().addMessageChat(std::string(data_deserialize, len));
			break;
		case SNAKE_ARRAY: {
			std::memcpy(snake_array, data_deserialize, sizeof(Snake) * MAX_SNAKE);
			break;
		}
		case SNAKE: {
			Snake snake_temp = *reinterpret_cast<Snake *>(data_deserialize);
			snake_array[snake_temp.id] = snake_temp;
			std::cout << snake_array[snake_temp.id] << std::endl;
			break;
		}
		case FOOD:
			int position[2];
			std::memcpy(position, data_deserialize, ClientTCP::size_header[FOOD]);
			factory.create_food(position[0], position[1]);
			break;
		case ID:
			id_ = *reinterpret_cast<int16_t const *>(data_deserialize);
			break;
		case START_GAME:break;
		case HEADER:break;
	}
	delete[] data_deserialize;
}

void ClientTCP::handle_read_data(eHeader header, boost::system::error_code const &error_code,
								 size_t len) {
	std::cout << "ClientTCP::handle_read_data [" << len << "]"
	<< "Header [" << header<< "]"<< std::endl;
	if (error_code.value() == 0 && len > 0) {
		parse_input(header, buffer_data.data(), len);
	}
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
	return snake_array;
}

void ClientTCP::food() {
	std::string buffer;
	int pos[2] = { 42, 42 };
	ClientTCP::add_prefix(FOOD, buffer, pos, ClientTCP::size_header[FOOD]);
	write_socket(buffer);
}
