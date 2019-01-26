#include <logger.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <nibbler.hpp>
#include <boost/asio.hpp>
#include <Univers.hpp>
#include "ClientTCP.hpp"
#include <gui/Core.hpp>
#include <exception>
#include <events/NextFrame.hpp>
#include <KINU/World.hpp>
#include <events/FoodCreation.hpp>
#include <events/StartEvent.hpp>
#include <systems/RenderSystem.hpp>

int const ClientTCP::size_header[] = {
		[static_cast<int>(eHeader::CHAT)] = SIZEOF_CHAT_PCKT,
		[static_cast<int>(eHeader::FOOD)] = sizeof(FoodInfo),
		[static_cast<int>(eHeader::ID)] = sizeof(int16_t),
		[static_cast<int>(eHeader::OPEN_GAME)] = sizeof(bool),
		[static_cast<int>(eHeader::START_GAME)] = sizeof(StartInfo),
		[static_cast<int>(eHeader::SNAKE)] = sizeof(Snake),
		[static_cast<int>(eHeader::HEADER)] = sizeof(eHeader),
		[static_cast<int>(eHeader::INPUT)] = sizeof(InputInfo),
		[static_cast<int>(eHeader::RESIZE_MAP)] = sizeof(unsigned int),
		[static_cast<int>(eHeader::REMOVE_SNAKE)] = sizeof(int16_t),
		[static_cast<int>(eHeader::POCK)] = sizeof(char),
		[static_cast<int>(eHeader::BORDERLESS)] = sizeof(bool),
		[static_cast<int>(eHeader::kPause)] = sizeof(eAction),
		[static_cast<int>(eHeader::kForcePause)] = sizeof(int16_t)
};

ClientTCP::ClientTCP(Univers &univers, bool fromIA)
		: fromIA_(fromIA),
		  openGame_(false),
		  id_(0),
		  univers(univers),
		  resolver(io),
		  socket(io),
		  factory(univers) {
}

ClientTCP::~ClientTCP() {
	log_error("Close clientTCP");
	io.stop();
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
		dns_ = dns;
		port_ = port;
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
														ClientTCP::size_header[static_cast<int>(eHeader::HEADER)]),
							boost::bind(&ClientTCP::handle_read_header,
										this,
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::read_socket_data(eHeader header) {
	boost::asio::async_read(socket, boost::asio::buffer(buffer_data,
														ClientTCP::size_header[static_cast<int>(header)]),
							boost::bind(&ClientTCP::handle_read_data,
										this,
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
										 this,
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}



void ClientTCP::parse_input(eHeader header, void const *input, size_t len) {
	mutex.lock();
	switch (header) {
		case eHeader::CHAT: {
			if (accept_data()) {

				char *data_deserialize = new char[len];
				std::memcpy(data_deserialize, input, len);
				univers.getCore_().addMessageChat(
						std::string(data_deserialize, len));
				delete[] data_deserialize;
			}
			break;
		}
		case eHeader::SNAKE: {
			if (accept_data()) {
				Snake snake_temp;
				std::memcpy(&snake_temp, input, len);
				snake_array_[snake_temp.id] = snake_temp;
				univers.playNoise(eSound::READY);
			}
			break;
		}
		case eHeader::REMOVE_SNAKE: {
			log_info("eHeader::REMOVE_SNAKE");
			snake_array_[*(reinterpret_cast< const int16_t *>(input))].reset();
			univers.playNoise(eSound::DEATH);
			break;
		}
		case eHeader::FOOD: {
	//		log_info("eHeader::FOOD");
			FoodInfo foodInfo;
			std::memcpy(&foodInfo, input, len);
			foodCreations.push_back(FoodCreation(foodInfo.positionComponent,
												 foodInfo.fromSnake));
			break;
		}
		case eHeader::ID: {
			std::memcpy(&id_, input, len);
			snake_array_[id_].id = id_;
			log_info("eHeader::ID %d", id_);
			break;
		}
		case eHeader::OPEN_GAME: {
			log_info("eHeader::OPEN_GAME");
			if (accept_data()) {
				ClientTCP::StartInfo startInfo;
				bool data;
				std::memcpy(&data, input,
							ClientTCP::size_header[static_cast<int>(eHeader::OPEN_GAME)]);
				openGame_ = data;
			}
			break;
		}
		case eHeader::START_GAME: {
			log_info("eHeader::START_GAME : %d", fromIA_);
			if (accept_data()) {
				StartInfo st;
				std::memcpy(&st, input,
							ClientTCP::size_header[static_cast<int>(eHeader::START_GAME)]);
				factory.create_all_snake(snake_array_, st.nu);

				if (univers.isServer()) {

					uint16_t  nu_ = std::count_if(snake_array_.begin(), snake_array_.end(), [](auto snake){ return snake.id != -1;});
					int max_food = (nu_ > 1 ? nu_ - 1 : nu_);
					log_warn("Number of food %d", max_food);

					for (int index = 0; index < max_food; ++index) {
						ClientTCP::FoodInfo foodInfo(
								PositionComponent(univers.getWorld_()
								.grid.getRandomSlot(eSprite::NONE))
								, false);
						write_socket(ClientTCP::add_prefix(eHeader::FOOD, &foodInfo));
					}

					univers.getWorld_().getEventsManager().emitEvent<StartEvent>(
							st.time_duration);
				}

			}
			break;
		}
		case eHeader::INPUT: {
			break;
		}
		case eHeader::RESIZE_MAP: {
			if (accept_data()) {
				log_info("eHeader::RESIZE_MAP");
				unsigned int buffer;
				std::memcpy(&buffer, input,
							ClientTCP::size_header[static_cast<int>(eHeader::RESIZE_MAP)]);
				univers.setMapSize(buffer);
				univers.playNoise(eSound::RESIZE_MAP);
			}
			break;
		}
		case eHeader::BORDERLESS : {
			if (accept_data()) {
				bool borderless;
				std::memcpy(&borderless, input,
							ClientTCP::size_header[static_cast<int>(eHeader::BORDERLESS)]);
				univers.setBorderless(borderless);
			}
			break;
		}
		case eHeader::POCK: {
			if (accept_data()) {
				univers.getWorld_().getEventsManager().emitEvent<NextFrame>();
			}
			break;
		}
		case eHeader::kPause : {
			univers.refreshTimerLoopWorld();
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

const std::string &ClientTCP::getDns() const {
	return dns_;
}

const std::string &ClientTCP::getPort() const {
	return port_;
}

/** Game Management **/

void ClientTCP::addScore(uint16_t score) {
}

void ClientTCP::send_host_open_game(void) {
	bool data;
	data = true;
	write_socket(add_prefix(eHeader::OPEN_GAME, &data));
}

void ClientTCP::change_map_size(unsigned int size) {
	write_socket(add_prefix(eHeader::RESIZE_MAP, &size));
}

void ClientTCP::change_name(char const *name) {
	bzero(snake_array_[id_].name, NAME_BUFFER);
	if (strlen(name) > NAME_BUFFER)
		strncpy(snake_array_[id_].name, name, NAME_BUFFER - 1);
	else
		strcpy(snake_array_[id_].name, name);
	refreshMySnake();
}

void ClientTCP::change_sprite(eSprite snakeSprite) {
	snake_array_[id_].sprite = snakeSprite;
	refreshMySnake();
}

void ClientTCP::change_state_ready(void) {
	snake_array_[id_].isReady = !snake_array_[id_].isReady;
	refreshMySnake();
}

void ClientTCP::refreshMySnake(void) {
	write_socket(add_prefix(eHeader::SNAKE, &snake_array_[id_]));
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
	return snake_array_.data();
}

Snake	const &ClientTCP::getSnake(void) const {
	return snake_array_[id_];
}

void ClientTCP::killSnake(uint16_t id) {
	log_warn("ClientTCP::killSnake. %d", id);
	auto snake  = snake_array_.__elems_[id];
	snake.isAlive = false;
	write_socket(add_prefix(eHeader::SNAKE, &snake));
}

void ClientTCP::send_borderless(bool borderless) {
	write_socket(ClientTCP::add_prefix(eHeader::BORDERLESS, &borderless));
}

bool ClientTCP::all_snake_is_dead() {
	return std::all_of(snake_array_.begin(), snake_array_.end(), [](auto snake){
		return !(snake.id != -1 && snake.isAlive);});
}

/** Mutex Management **/

void ClientTCP::lock() {
	mutex.lock();
}

void ClientTCP::unlock() {
	mutex.unlock();
}

bool ClientTCP::accept_data() {
	return !fromIA_ || (univers.isOnlyIA() && id_ == 0);
}

bool ClientTCP::isSwitchingLibrary() const {
	return snake_array_[id_].isSwitchingLibrary;
}

ClientTCP::FoodInfo::FoodInfo() {

}

ClientTCP::FoodInfo::FoodInfo(PositionComponent component, bool from) {
	positionComponent = component;
	fromSnake = from;
}
