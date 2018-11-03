#include <cstdlib>
#include <deque>
#include <iostream>
#include <nibbler.hpp>
#include "asio.hpp"
#include "ClientTCP.hpp"
#include <gui/Core.hpp>

ClientTCP::ClientTCP(Univers &univers, boost::asio::io_service &io,
					 std::string &hostname)
		: id_(0),
		  univers(univers),
		  resolver(io),
		  query(hostname, "4242"),
		  socket(io),
		  timer(io, boost::posix_time::seconds(1)) {
}

void ClientTCP::connect() {
	std::cout << "Trying to connect" << std::endl;
	tcp::resolver::iterator it = resolver.resolve(query);
	boost::asio::connect(socket, it);
}

void ClientTCP::read_socket() {
	boost::asio::async_read_until(socket, streambuf_, '\n',
								  boost::bind(&ClientTCP::handle_read,
											  shared_from_this(),
											  boost::asio::placeholders::error,
											  boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::write_socket(std::string message) {
	boost::asio::async_write(socket, boost::asio::buffer(message),
							 boost::bind(&ClientTCP::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void ClientTCP::add_prefix(eHeader header, std::string &message) {
	char *header_serialized = reinterpret_cast<char *>(&header);
	message.insert(0, header_serialized, sizeof(eHeader));
	message += "\n";
}


void ClientTCP::parse_input(void const *input, size_t len) {

	eHeader header;
	size_t header_len = sizeof(eHeader);
	std::memcpy(&header, input, header_len);
	char const *data_deserialize = reinterpret_cast<char const *>(input) +
			header_len;
	switch (header) {
		case CHAT: {
			this->univers.getCore_().addMessageChat(
					std::string(data_deserialize, len - header_len));
			break;
		}
		case SNAKE_ARRAY:
		{
			std::memcpy(snakes, data_deserialize, sizeof(Snake) * MAX_SNAKE);
			for(int i=0;i<MAX_SNAKE;i++)
				std::cout << this->snakes[i] << std::endl;
			break;
		}
		case SNAKE:
		{
			int16_t			id;
			int	index;

			id = *reinterpret_cast<int16_t const *>(data_deserialize);
			index = Snake::getSnakeById(snakes, MAX_SNAKE, id);
			std::cout << "Recv: index[" <<index << "] id [" << id << "]" << std::endl;
			if (index == -1 && Snake::isFull(snakes, MAX_SNAKE))
				break;
			else if (index == -1)
				index = Snake::getlastSnakeIndex(snakes, MAX_SNAKE);
			std::memcpy(&snakes[index], data_deserialize + sizeof(id), sizeof(Snake));
			for(int i=0;i<MAX_SNAKE;i++)
				std::cout << this->snakes[i] << std::endl;
			break;
		}
		case FOOD:
			break;
		case ID:
			id_ = *(reinterpret_cast<int16_t const *>(data_deserialize));
			std::cout << id_ << std::endl;
			break;
	}
}

void ClientTCP::handle_read(
		boost::system::error_code const &error_code, size_t len) {
	if (error_code.value() == 0 && len > 0) {
		parse_input(streambuf_.data().data(), len);
	}
	streambuf_.consume(len);
	read_socket();
}

void ClientTCP::handle_write(const boost::system::error_code &error_code,
							 size_t len) {
}

ClientTCP::pointer_client
ClientTCP::create(Univers &univers, boost::asio::io_service &io,
				  std::string hostname) {
	return pointer_client(new ClientTCP(univers, io, hostname));
}

Snake const *ClientTCP::getSnakes() const {
	return snakes;
}
