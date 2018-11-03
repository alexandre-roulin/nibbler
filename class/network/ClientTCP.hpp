#pragma once


#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include <Univers.hpp>
#include "ServerTCP.hpp"
#include "chat_message.hpp"

using boost::asio::ip::tcp;

class ClientTCP : public boost::enable_shared_from_this<ClientTCP> {
public:
	typedef boost::shared_ptr<ClientTCP> pointer_client;


	ClientTCP(Univers &univers, boost::asio::io_service &io, std::string &hostname);

	void read_socket();
	void connect();
	void write_socket(std::string message);

	void handle_read(const boost::system::error_code &, size_t);

	void handle_write(const boost::system::error_code & /*error*/,
					  size_t /*bytes_transferred*/);

	static void add_prefix(eHeader, std::string &);

	static pointer_client
	create(Univers &univers, boost::asio::io_service &io, std::string hostname);

	void parse_input(void const *, size_t);

private:
public:
	const Snake *getSnakes() const;

private:
	int16_t id_;
	Snake const *snakes;
	boost::asio::streambuf streambuf_;
	boost::asio::deadline_timer timer;
	tcp::socket socket;
	tcp::resolver resolver;
	tcp::resolver::query query;
	Univers	&univers;
};
