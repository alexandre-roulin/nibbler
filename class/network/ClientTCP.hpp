#pragma once


#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include "ServerTCP.hpp"
#include "chat_message.hpp"

using boost::asio::ip::tcp;

class ClientTCP : public boost::enable_shared_from_this<ClientTCP> {
public:
	typedef boost::shared_ptr<ClientTCP> pointer_client;


	ClientTCP(boost::asio::io_service &io, std::string &hostname);

	void read_socket();

	void write_socket(std::string message);

	void handle_read(const boost::system::error_code &, size_t);

	void handle_write(const boost::system::error_code & /*error*/,
					  size_t /*bytes_transferred*/);
	static pointer_client create(boost::asio::io_service &io, std::string hostname);

private:
	boost::asio::streambuf b;
	boost::asio::deadline_timer timer;
	tcp::socket socket;
	tcp::resolver resolver;
	tcp::resolver::query query;
};


