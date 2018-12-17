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
#include <component/PositionComponent.hpp>
#include <factory/Factory.hpp>
#include "IGameNetwork.hpp"

class JoystickEvent;

class StartEvent;

class FoodCreation;

using boost::asio::ip::tcp;

class Univers;

class ClientTCP : public boost::enable_shared_from_this<ClientTCP>, public IGameNetwork {
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

	static pointer_client create(Univers &univers, bool fromIA);

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

	virtual void addScore(uint16_t uint16);

	virtual void deliverEvents();

	virtual void refreshMySnake(void);

	virtual void send_host_open_game(void);

	virtual void send_borderless(bool borderless);

	virtual void change_name(char const *name);

	virtual void change_sprite(eSprite snakeSprite);

	virtual void change_state_ready(void);

	virtual void change_map_size(unsigned int i);

	virtual bool all_snake_is_dead();

	virtual const Snake *getSnakes() const;

	virtual const Snake &getSnake(void) const;

	virtual int16_t getId(void) const;

	virtual void killSnake(uint16_t);

public:
	/** Mutex Management **/

	void lock();

	void unlock();


	static const int size_header[];

private:

	ClientTCP(Univers &univers, bool fromIA);

	void checkError_(boost::system::error_code const &);

	boost::asio::io_service io;
	bool openGame_;
	bool fromIA_;
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
	std::string dns_;
public:
	const std::string &getDns() const;

	const std::string &getPort() const;

private:
	std::string port_;
};


template<typename T>
std::string ClientTCP::add_prefix(eHeader header, T *element) {
	std::string message;
	message.append(reinterpret_cast<char *>(&header), sizeof(eHeader));
	message.append(reinterpret_cast<char *>(element), size_header[header]);
	return message;
}
