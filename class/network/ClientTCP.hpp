#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/array.hpp>
#include <nibbler.hpp>
#include <factory/Factory.hpp>
#include <events/JoystickEvent.hpp>
#include <events/StartEvent.hpp>
#include <events/FoodCreation.hpp>
using boost::asio::ip::tcp;

class Univers;

class Factory;

class ClientTCP : public boost::enable_shared_from_this<ClientTCP> {
public:
	typedef boost::shared_ptr<ClientTCP> pointer_client;

	struct StartInfo {
		unsigned int nu;
		boost::posix_time::ptime time_duration;
	};


	struct InputInfo {
		int16_t id;
		eDirection dir;
	};

	void deliverEvents();

	void connect(std::string dns, std::string port);

	void read_socket_header();

	void read_socket_data(eHeader);

	void write_socket(std::string message);

	void handle_read_data(eHeader, const boost::system::error_code &, size_t);

	void handle_read_header(const boost::system::error_code &, size_t);

	void handle_write(const boost::system::error_code &, size_t);

	void parse_input(eHeader header, void const *, size_t);

	void refreshMySnake(void);

	void send_host_open_game(void);
	
	void change_name(char const *name);

	void change_sprite(eSprite snakeSprite);

	void change_state_ready(void);

	void change_map_size(unsigned int);

	template<typename T>
	static std::string add_prefix(eHeader, T *element);

	template<typename T, typename U>
	static std::string add_prefix(eHeader, T *element1, U *element2);

	static pointer_client
	create(Univers &univers, boost::asio::io_service &io);

	const Snake *getSnakes() const;

	Snake const &getSnake(void) const;

	int16_t getId(void) const;
	bool	isConnect() const;
	bool	isOpenGame() const;

	static int const size_header[];
private:
	ClientTCP(::Univers &univers, boost::asio::io_service &io);

	bool isConnect_;
	bool openGame_;
	std::vector<FoodCreation> foodCreations;
	std::vector<JoystickEvent> joystickEvents;
	std::vector<StartEvent> startEvents;
	Snake snake_array[MAX_SNAKE];
	tcp::socket socket;
	boost::array<char, 512> buffer_data;
	int16_t id_;
	tcp::resolver resolver;
	Univers &univers;
	Factory factory;
	boost::asio::io_service &io;
	boost::thread thread;
	std::__thread_id id;
	std::mutex mu;

};

template<typename T, typename U>
std::string ClientTCP::add_prefix(eHeader header, T *element1, U *element2) {
	std::string message;
	message.append(reinterpret_cast<char *>(&header), sizeof(eHeader));
	message.append(reinterpret_cast<char *>(element1), sizeof(*element1));
	message.append(reinterpret_cast<char *>(element2), sizeof(*element2));
	return message;
}

template<typename T>
std::string ClientTCP::add_prefix(eHeader header, T *element) {
	std::string message;
	message.append(reinterpret_cast<char *>(&header), sizeof(eHeader));
	message.append(reinterpret_cast<char *>(element),
				   ClientTCP::size_header[header]);
	return message;
}
