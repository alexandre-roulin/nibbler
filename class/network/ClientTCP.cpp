#include <cstdlib>
#include <deque>
#include <iostream>
#include <nibbler.hpp>
#include "asio.hpp"
#include "ClientTCP.hpp"
#include <gui/Core.hpp>
#include <events/JoystickEvent.hpp>
#include <events/StartEvent.hpp>

int const ClientTCP::size_header[] = {
		[CHAT] = SIZEOF_CHAT_PCKT,
		[FOOD] = sizeof(int) * 2,
		[ID] = sizeof(int16_t),
		[START_GAME] = sizeof(int16_t),
		[SNAKE] = sizeof(Snake),
		[SNAKE_ARRAY] = sizeof(Snake) * MAX_SNAKE,
		[HEADER] = sizeof(eHeader),
		[INPUT] = sizeof(int16_t) + sizeof(eDirection)
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

void ClientTCP::change_name(char const *name) {
	bzero(snake_array[id_].name, NAME_BUFFER);
	if (strlen(name) > NAME_BUFFER)
		strncpy(snake_array[id_].name, name, NAME_BUFFER - 1);
	else
		strcpy(snake_array[id_].name, name);
	this->refreshMySnake();
}

void ClientTCP::change_sprite(eSnakeSprite snakeSprite) {
	snake_array[id_].sprite = snakeSprite;
	this->refreshMySnake();
}

void ClientTCP::change_state_ready(void) {
	snake_array[id_].isReady = !snake_array[id_].isReady;
	this->refreshMySnake();
}

void ClientTCP::refreshMySnake(void) {
	write_socket(add_prefix(SNAKE, &snake_array[id_]));
}

void ClientTCP::connect() {
	tcp::resolver::iterator it = resolver.resolve(query);
	boost::asio::connect(socket, it);
}

void ClientTCP::read_socket_header() {

//	std::cout << "ClientTCP::read_socket_header" << std::endl;
	boost::asio::async_read(socket, boost::asio::buffer(buffer_data,
														ClientTCP::size_header[HEADER]),
							boost::bind(&ClientTCP::handle_read_header,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::read_socket_data(eHeader header) {
//	std::cout << "ClientTCP::read_socket_data" << std::endl;
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
//	std::cout << "ClientTCP::handle_read_header > len : " << len << std::endl;
	if (error_code.value() == 0) {
		assert(len == sizeof(eHeader));
		eHeader header;
		std::memcpy(&header, buffer_data.data(), sizeof(eHeader));
//		std::cout << "Header : " << header << std::endl;
		read_socket_data(header);
	}
}

int16_t ClientTCP::getId_() const {
	return id_;
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



	switch (header) {
		case CHAT: {
			log_info("CHAT");
			char *data_deserialize = new char[len];
			std::memcpy(data_deserialize, input, len);
			univers.getCore_().addMessageChat(std::string(data_deserialize, len));
			delete [] data_deserialize;
			break;
		}
		case SNAKE_ARRAY: {
			log_info("SNAKE_ARRAY");
			std::memcpy(snake_array, input, sizeof(Snake) * MAX_SNAKE);
			break;
		}
		case SNAKE: {
			log_info("SNAKE");

			Snake snake_temp;

			std::memcpy(&snake_temp, input, len);

			snake_array[snake_temp.id] = snake_temp;

			break;
		}
		case FOOD: {
			log_info("FOOD");
			int position[2];
			std::memcpy(position, input, len);
			factory.create_food(position[0], position[1]);
			break;
		}

		case ID:
			log_info("ID");
			std::memcpy(&id_, input, len);
			break;
		case START_GAME: {
			log_info("START_GAME");
			int16_t nu;
			std::memcpy(&nu, input, ClientTCP::size_header[START_GAME]);
			factory.create_all_snake(snake_array, nu);
			univers.getWorld_().getEventManager().emitEvent<StartEvent>();

		}
			break;
		case INPUT: {
			log_info("INPUT");
			eDirection dir;
			int16_t id;
			char *data_deserialize = new char[len];
			std::memcpy(data_deserialize, input, len);

			std::memcpy(&id, data_deserialize, sizeof(int16_t));
			std::memcpy(&dir, data_deserialize + sizeof(int16_t), sizeof(eDirection));

			univers.getWorld_().getEventManager().emitEvent<JoystickEvent>(id, dir);

//			std::cout << "Size: " << univers.getWorld_().getEventManager().getEvents<JoystickEvent>().size() << std::endl;

			delete [] data_deserialize;
		}
			break;
		default:
			break;
	}
}



void ClientTCP::handle_read_data(eHeader header, boost::system::error_code const &error_code,
								 size_t len) {
//	std::cout << "ClientTCP::handle_read_data [" << len << "]"
//	<< "Header [" << header<< "]"<< std::endl;
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
Snake	const &ClientTCP::getSnake(void) const {
	return this->snake_array[this->id_];
}

int16_t		ClientTCP::getId(void) const {
	return this->id_;
}

