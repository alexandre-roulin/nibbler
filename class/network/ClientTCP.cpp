#include <logger.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <nibbler.hpp>
#include "asio.hpp"
#include <Univers.hpp>
#include "ClientTCP.hpp"
#include <gui/Core.hpp>
#include <exception>
#include <events/NextFrame.hpp>

int const ClientTCP::size_header[] = {
		[CHAT] = SIZEOF_CHAT_PCKT,
		[FOOD] = sizeof(FoodInfo),
		[ID] = sizeof(int16_t),
		[OPEN_GAME] = sizeof(bool),
		[START_GAME] = sizeof(StartInfo),
		[SNAKE] = sizeof(Snake),
		[SNAKE_ARRAY] = sizeof(Snake) * MAX_SNAKE,
		[HEADER] = sizeof(eHeader),
		[INPUT] = sizeof(InputInfo),
		[RESIZE_MAP] = sizeof(unsigned int),
		[REMOVE_SNAKE] = sizeof(int16_t),
		[POCK] = sizeof(char),
		[BORDERLESS] = sizeof(bool),
		[DISCONNECT] = sizeof(bool)

};

ClientTCP::ClientTCP(Univers &univers)
		: openGame_(false),
		  id_(0),
		  univers(univers),
		  resolver(io),
		  socket(io),
		  factory(univers) {
}

ClientTCP::pointer_client

ClientTCP::create(Univers &univers) {
	return pointer_client(new ClientTCP(univers));
}

ClientTCP::~ClientTCP() {
	socket.close();
}

/** Network Management **/

void ClientTCP::connect(std::string dns, std::string port) {
	try {
		tcp::resolver::query query(dns, port);
		tcp::resolver::iterator it = resolver.resolve(query);
		boost::asio::connect(socket, it);
		read_socket_header();
		thread = boost::thread(boost::bind(&boost::asio::io_service::run, &io));
		thread.detach();
	} catch (std::exception &exception) {
		std::cout << exception.what() << std::endl;
	}

}

void ClientTCP::checkError_(boost::system::error_code const &error_code) {

	if ((boost::asio::error::eof == error_code) ||
		(boost::asio::error::connection_reset == error_code))
	{
		thread.interrupt();
		socket.close();
//		throw std::runtime_error("Lost signal from server");
	}
}


void ClientTCP::read_socket_header() {
	boost::asio::async_read(socket, boost::asio::buffer(buffer_data,
														ClientTCP::size_header[HEADER]),
							boost::bind(&ClientTCP::handle_read_header,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::read_socket_data(eHeader header) {
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

	checkError_(error_code);

	if (error_code.value() == 0) {
		assert(len == sizeof(eHeader));
		eHeader header;
		std::memcpy(&header, buffer_data.data(), sizeof(eHeader));
		read_socket_data(header);
	}
}


void ClientTCP::write_socket(std::string message) {
	boost::asio::async_write(socket, boost::asio::buffer(message),
							 boost::bind(&ClientTCP::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::parse_input(eHeader header, void const *input, size_t len) {
	mutex.lock();
	switch (header) {
		case CHAT: {
			char *data_deserialize = new char[len];
			std::memcpy(data_deserialize, input, len);
			univers.getCore_().addMessageChat(std::string(data_deserialize, len));
			delete [] data_deserialize;
			break;
		}
		case SNAKE_ARRAY: {
			std::memcpy(snake_array, input, len);
			break;
		}
		case SNAKE: {
			Snake snake_temp;
			std::memcpy(&snake_temp, input, len);
			snake_array[snake_temp.id] = snake_temp;
			univers.playNoise(eSound::READY);
			break;
		}
		case REMOVE_SNAKE: {
			log_info("eHeader::REMOVE_SNAKE");
			snake_array[*(reinterpret_cast< const int16_t *>(input))].id = -1;
			univers.playNoise(eSound::DEATH);
			break;
		}
		case FOOD: {
			log_info("eHeader::FOOD");
			FoodInfo foodInfo;
			std::memcpy(&foodInfo, input, len);
			foodCreations.push_back(FoodCreation(foodInfo.positionComponent, foodInfo.fromSnake));
			break;
		}
		case ID: {
			log_info("eHeader::ID");
			std::memcpy(&id_, input, len);
			break;
		}
		case OPEN_GAME: {
			log_info("eHeader::OPEN_GAME");
			ClientTCP::StartInfo startInfo;
			bool data;
			std::memcpy(&data, input, ClientTCP::size_header[OPEN_GAME]);
			openGame_ = data;
			break;
		}
		case START_GAME: {
			log_info("eHeader::START_GAME");
			StartInfo st;
			std::memcpy(&st, input, ClientTCP::size_header[START_GAME]);
			factory.create_all_snake(snake_array, st.nu);
			univers.getWorld_().getEventsManager().emitEvent<StartEvent>(st.time_duration);
			break;
		}
		case INPUT: {
			break;
		}
		case RESIZE_MAP: {
			log_info("eHeader::RESIZE_MAP");
			unsigned int buffer;
			std::memcpy(&buffer, input, ClientTCP::size_header[RESIZE_MAP]);
			univers.setMapSize(buffer);
			univers.playNoise(eSound::RESIZE_MAP);
			break;
		}
		case BORDERLESS : {
			bool borderless;
			std::memcpy(&borderless, input, ClientTCP::size_header[BORDERLESS]);
			univers.setBorderless(borderless);
			break;
		}
		case POCK: {
			univers.getWorld_().getEventsManager().emitEvent<NextFrame>();
			break;
		}
		default:
			break;
	}
	mutex.unlock();

}
void ClientTCP::handle_read_data(eHeader header, boost::system::error_code const &error_code,
								 size_t len) {
	checkError_(error_code);
	if (error_code.value() == 0 && len > 0) {
		parse_input(header, buffer_data.data(), len);
	}
	read_socket_header();
}

void ClientTCP::handle_write(const boost::system::error_code &error_code,
							 size_t) {
	checkError_(error_code);
}

void ClientTCP::close_connection() {
	bool data = true;
	write_socket(ClientTCP::add_prefix(DISCONNECT, &data));
}

/** Game Management **/

void ClientTCP::send_host_open_game(void) {
	bool data;
	data = true;
	write_socket(add_prefix(OPEN_GAME, &data));
}

void ClientTCP::change_map_size(unsigned int size) {
	write_socket(add_prefix(RESIZE_MAP, &size));
}

void ClientTCP::change_name(char const *name) {
	bzero(snake_array[id_].name, NAME_BUFFER);
	if (strlen(name) > NAME_BUFFER)
		strncpy(snake_array[id_].name, name, NAME_BUFFER - 1);
	else
		strcpy(snake_array[id_].name, name);
	this->refreshMySnake();
}

void ClientTCP::change_sprite(eSprite snakeSprite) {
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

int16_t ClientTCP::getId() const {
	return id_;
}

bool	ClientTCP::isConnect() const {
	return socket.is_open();
}

bool	ClientTCP::isOpenGame() const {
	return openGame_;
}

void ClientTCP::deliverEvents() {
	mutex.lock();
	for (auto foodCreation : foodCreations) {
		univers.getWorld_().getEventsManager().emitEvent(foodCreation);
	}
	foodCreations.clear();
	mutex.unlock();
}

Snake const *ClientTCP::getSnakes() const {
	return snake_array;
}

Snake	const &ClientTCP::getSnake(void) const {
	return this->snake_array[this->id_];
}

void ClientTCP::killSnake() {
	log_warn("ClientTCP::killSnake.%d", getId());
	snake_array[id_].isAlive = false;
	write_socket(add_prefix(SNAKE, &(snake_array[id_])));
}

void ClientTCP::send_borderless(bool borderless) {
	write_socket(ClientTCP::add_prefix(BORDERLESS, &borderless));
}

bool ClientTCP::all_snake_is_dead() {
	for (int index = 0; index < MAX_SNAKE; ++index)
		if (snake_array[index].id != -1 && snake_array[index].isAlive)
			return false;
	return true;
}

/** Mutex Management **/

void ClientTCP::lock() {
	mutex.lock();
}

void ClientTCP::unlock() {
	mutex.unlock();
}
