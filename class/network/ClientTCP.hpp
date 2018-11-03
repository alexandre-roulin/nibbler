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


	ClientTCP(Univers &univers, boost::asio::io_service &io,
			  std::string &hostname);

	void connect();

	void read_socket();

	void change_state_ready();

	void write_socket(std::string message);
	void write_socket(void const *data, size_t len);

	void parse_input(void const *, size_t);

	void handle_read(const boost::system::error_code &, size_t);

	void handle_write(const boost::system::error_code &, size_t);

	template<typename T>
	static void add_prefix(eHeader, std::string &, T *element, size_t bytes);

	static pointer_client
	create(Univers &univers, boost::asio::io_service &io, std::string hostname);


private:
public:
	const Snake *getSnakes() const;

private:
	int16_t id_;
	Snake snakes[MAX_SNAKE];
	boost::asio::streambuf streambuf_;
	boost::asio::deadline_timer timer;
	tcp::socket socket;
	tcp::resolver resolver;
	tcp::resolver::query query;
	Univers &univers;
};

template<typename T>
void ClientTCP::add_prefix(eHeader header, std::string &message, T *element,
						   size_t bytes) {
	char *header_serialized = reinterpret_cast<char *>(&header);
	message.insert(0, header_serialized, sizeof(eHeader));
	message.insert(sizeof(eHeader), reinterpret_cast<char *>(element), bytes);
	message.insert(sizeof(eHeader) + bytes, "\n", 1);
}