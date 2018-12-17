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

	TCPConnection(Snake const &, boost::asio::io_service &io_service, ServerTCP &serverTCP);

	void checkError_(boost::system::error_code const &error_code);


public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	static pointer
	create(Snake const &snake, boost::asio::io_service &io_service, ServerTCP &serverTCP);
	void read_socket_header();

	void read_socket_data(eHeader);

	void write_socket(std::string message);

	void write_socket(void const *data, size_t len);

	void handle_read_data(eHeader header, const boost::system::error_code &, size_t);

	void handle_read_header(const boost::system::error_code &, size_t);

	void handle_write(const boost::system::error_code &, size_t);

	void close();

	tcp::socket &socket();
};


class ServerTCP {
public:
	ServerTCP(unsigned int port);

	virtual ~ServerTCP();

	void async_write(std::string message);

	void async_write(void const *input, size_t len);

	void parse_input(eHeader, void const *, size_t);

	void erase_snake(Snake const &);

	void refresh_data_snake_array(TCPConnection::pointer &, int16_t);

	void refresh_data_map_size(TCPConnection::pointer &connection);

	void start_game();

	void close_acceptor();

	void remove(TCPConnection::pointer);

	bool isFull() const;

private:
	int16_t nu_;
	Snake snake_array[MAX_SNAKE];
	unsigned int	mapSize;
	std::vector<ClientTCP::FoodInfo> foodInfoArray;
	std::mutex mutex;
	void start_accept();

	std::vector<TCPConnection::pointer> pointers;
	boost::asio::io_service io_service_;
	tcp::acceptor acceptor_;
	boost::thread thread;
};

