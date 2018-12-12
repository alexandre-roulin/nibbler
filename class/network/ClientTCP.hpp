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

	struct FoodInfo {
		bool fromSnake;
		PositionComponent positionComponent;

	};

	static pointer_client create(Univers &univers);

	virtual ~ClientTCP();

	/** Network Management **/

	void connect(std::string dns, std::string port);

	void read_socket_header();

	void read_socket_data(eHeader);

	void write_socket(std::string message);

	void handle_read_data(eHeader, const boost::system::error_code &, size_t);

	void handle_read_header(const boost::system::error_code &, size_t);

	void handle_write(const boost::system::error_code &, size_t);

	void parse_input(eHeader header, void const *, size_t);

	bool isConnect() const;

	void close_connection();

	template<typename T>
	static std::string add_prefix(eHeader, T *element);

	bool isOpenGame() const;

	/** Game Management **/


	void deliverEvents();

	void refreshMySnake(void);

	void send_host_open_game(void);

	void send_borderless(bool borderless);

	void change_name(char const *name);

	void change_sprite(eSprite snakeSprite);

	void change_state_ready(void);

	void change_map_size(unsigned int);

	bool all_snake_is_dead();

	const Snake *getSnakes() const;

	Snake const &getSnake(void) const;

	int16_t getId(void) const;

	void killSnake();

	/** Mutex Management **/

	void lock();

	void unlock();


	static int const size_header[];

private:

	ClientTCP(Univers &univers);

	void checkError_(boost::system::error_code const &error_code);

	boost::asio::io_service io;
	bool openGame_;
	std::vector<FoodCreation> foodCreations;
	Snake snake_array[MAX_SNAKE];
	tcp::socket socket;
	boost::array<char, 512> buffer_data;
	int16_t id_;
	tcp::resolver resolver;
	Univers &univers;
	Factory factory;
	boost::thread thread;
	std::mutex mutex;
};

template<typename T>
std::string ClientTCP::add_prefix(eHeader header, T *element) {
	std::string message;
	message.append(reinterpret_cast<char *>(&header), sizeof(eHeader));
	message.append(reinterpret_cast<char *>(element),
				   ClientTCP::size_header[header]);
	return message;
}
