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
#include <nibbler.hpp>
#include <Univers.hpp>
#include "ClientTCP.hpp"

#define DATA '#'

using boost::asio::ip::tcp;

class ServerTCP;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
private:
	Snake const &snake_;
	tcp::socket socket_;
	boost::array<char, 512> buffer_data;
	ServerTCP &serverTCP_;

public:

private:
	TCPConnection(Snake const &, boost::asio::io_service &io_service, ServerTCP &serverTCP);

	void checkError_(boost::system::error_code const &error_code);

public:

	virtual ~TCPConnection();

	int16_t getId() const;

	typedef boost::shared_ptr<TCPConnection> pointer;

	static pointer
	create(Snake const &snake, boost::asio::io_service &io_service, ServerTCP &serverTCP);

	void readSocketHeader();

	void readSocketData(eHeader header);

	void writeSocket(std::string message);

	void handle_read_data(eHeader header, const boost::system::error_code &, size_t);

	void handle_read_header(const boost::system::error_code &, size_t);

	void handle_write(const boost::system::error_code &, size_t);


	tcp::socket &socket();
};


class ServerTCP {
public:

	ServerTCP() = delete;

	ServerTCP(Univers &univers, unsigned int port);

	~ServerTCP();

	void writeToClientTCP(std::string message);

	void parseInput(eHeader header, void const *input, size_t len);

	void eraseSnake(Snake const &snake);

	void sendSnakeArray();

	void startGame();

	void remove(TCPConnection::pointer);

	bool isFull() const;

	bool isReady() const;

	void updateInput();

	unsigned int getPort_() const;

private:

	friend class TCPConnection;

	unsigned int port_;

	uint16_t number_clients_;

	Univers &univers_;

	std::array<Snake, MAX_SNAKE> snake_array_;

	unsigned int mapSize;

	bool pause_;

	bool forcePause_;

	std::vector<ClientTCP::FoodInfo> foodInfoArray;

	std::mutex mutex;

	void start_accept();

	std::vector<TCPConnection::pointer> pointers;

	boost::asio::io_service io_service_;

	tcp::acceptor acceptor_;

	boost::thread thread;
};

