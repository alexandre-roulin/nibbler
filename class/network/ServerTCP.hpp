#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/thread.hpp>
#include <nibbler.hpp>
#include <Univers.hpp>
#include <boost/array.hpp>


using boost::asio::ip::tcp;

class ServerTCP;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
private:
	tcp::socket socket_;
	boost::asio::streambuf buffer_chat;
	boost::array<char, 512> buffer_data;
	ServerTCP &serverTCP_;

	TCPConnection(boost::asio::io_service &io_service, ServerTCP &serverTCP);



public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	static pointer
	create(boost::asio::io_service &io_service, ServerTCP &serverTCP);
	void read_socket_header();

	void read_socket_chat();

	void read_socket_data(eHeader);

	void write_socket(std::string message);

	void write_socket(void const *data, size_t len);

	void handle_read_data(const boost::system::error_code &, size_t);

	void handle_read_header(const boost::system::error_code &, size_t);

	void handle_read_chat(const boost::system::error_code &, size_t);

	void handle_write(const boost::system::error_code &, size_t);

	tcp::socket &socket();
};


class ServerTCP {
public:
	ServerTCP(Univers &univers, boost::asio::io_service &io_service);

	void async_write(std::string message);

	void async_write(void const *input, size_t len);

	void parse_input(eHeader, void const *, size_t);

	void refresh_data_snake_array(TCPConnection::pointer &, int16_t);

	void start_game();

	void remove(TCPConnection::pointer);

private:
	int16_t nu_;
	Snake snakes[MAX_SNAKE];

	void start_accept();

	Univers &univers;
	std::vector<TCPConnection::pointer> pointers;
	boost::thread thread_accept;
	boost::asio::deadline_timer timer_accept;
	tcp::acceptor acceptor_;
};
