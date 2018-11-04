#include "ServerTCP.hpp"
#include "ClientTCP.hpp"
#include <factory/Factory.hpp>

ServerTCP::ServerTCP(Univers &univers, boost::asio::io_service &io_service)
		: nu_(0),
		  univers(univers),
		  acceptor_(io_service, tcp::endpoint(tcp::v4(), 4242)),
		  timer_accept(io_service, boost::posix_time::seconds(1)) {
	std::cout << "Server created" << std::endl;
	start_accept();
}


void ServerTCP::start_accept() {
	acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {

		if (!ec) {
			TCPConnection::pointer new_connection =
					TCPConnection::create(acceptor_.get_io_service(), *this);
			new_connection->socket() = std::move(socket);
			snakes[nu_].id = nu_;
			snakes[nu_] = Snake::randomSnake(nu_);
			new_connection->read_socket_header();
			pointers.push_back(new_connection);
			refresh_data_snake_array(new_connection, nu_);
			++nu_;
		}
		start_accept();
	});
}

void ServerTCP::refresh_data_snake_array(
		TCPConnection::pointer &connection,
		int16_t id) {
	{
		std::string buffer;
		ClientTCP::add_prefix(ID, buffer, &id, sizeof(int16_t));
//		std::cout << "ServerTCP::refresh_data_snake_array : " << buffer.size() << std::endl;
		connection->write_socket(buffer);
	}
	{
		std::string buffer;
		ClientTCP::add_prefix(SNAKE_ARRAY, buffer, snakes,
				sizeof(Snake) * MAX_SNAKE);
//		std::cout << "ServerTCP::refresh_data_snake_array : " << buffer.size() << std::endl;
		connection->write_socket(buffer);
	}
	{
		std::string buffer;
		ClientTCP::add_prefix(SNAKE, buffer, &(snakes[id]), sizeof(Snake));
//		std::cout << "ServerTCP::refresh_data_snake_array : " << buffer.size() << std::endl;
		async_write(buffer);
	}
}

void ServerTCP::start_game() {
	for (int index = 0; index < MAX_SNAKE; ++index) {
		if (!snakes[index].isReady && snakes[index].id != -1)
			return;
	}
	Factory factory(univers.getWorld_());
	factory.create_all_snake(nu_);
}

void ServerTCP::async_write(std::string message) {
	for (auto &pointer : pointers) {
		pointer->write_socket(message);
	}
}

void ServerTCP::async_write(void const *input, size_t len) {
	for (auto &pointer : pointers) {
		pointer->write_socket(input, len);
	}
}

void ServerTCP::parse_input(void const *input, size_t len) {
	eHeader header;
	size_t header_len = sizeof(eHeader);
	std::memcpy(&header, input, header_len);
	char *data_deserialize = new char[len];
	std::memcpy(data_deserialize,
				reinterpret_cast<char const *>(input) + header_len,
				len - header_len);

	switch (header) {
		case SNAKE_ARRAY: {
			std::memcpy(snakes, data_deserialize, sizeof(Snake) * MAX_SNAKE);
			break;
		}
		case SNAKE: {
			Snake snake_temp = *reinterpret_cast<Snake *>(data_deserialize);
			snakes[snake_temp.id] = snake_temp;
			break;
		}
		default:
			break;
	}
	delete [] data_deserialize;
	async_write(input, len);
}

void ServerTCP::remove(TCPConnection::pointer remove) {
	pointers.erase(std::remove_if(pointers.begin(), pointers.end(),
								  [remove](TCPConnection::pointer pointer) {
									  return remove == pointer;
								  }));
}

/** ---------------------- TCPConnection ---------------------- **/

TCPConnection::TCPConnection(
		boost::asio::io_service &io_service,
		ServerTCP &serverTCP)
		: socket_(io_service),
		  serverTCP_(serverTCP) {
}

void TCPConnection::write_socket(std::string message) {
	boost::asio::async_write(socket_, boost::asio::buffer(message),
							 boost::bind(&TCPConnection::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::write_socket(void const *data, size_t bytes) {
	boost::asio::async_write(socket_, boost::asio::buffer(data, bytes),
							 boost::bind(&TCPConnection::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::read_socket() {

}


void TCPConnection::read_socket_header() {
	boost::asio::async_read(socket_, boost::asio::buffer(buffer_data, sizeof(eHeader)),
							boost::bind(&ClientTCP::handle_read_header,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::read_socket_data(eHeader header) {
//	std::cout << "TCPConnection::read_socket_data" << std::endl;
	boost::asio::async_read(socket_, boost::asio::buffer(buffer_data, header),
							boost::bind(&ClientTCP::handle_read_data,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}


 void TCPConnection::read_socket_chat() {
	 boost::asio::async_read_until(socket_, buffer_chat, '\n',
								   boost::bind(&TCPConnection::handler_read,
											   shared_from_this(),
											   boost::asio::placeholders::error,
											   boost::asio::placeholders::bytes_transferred));
}
void
TCPConnection::handle_write(const boost::system::error_code &error_code,
							size_t len) {
	if (error_code.value() != 0)
		std::cout << "Write : " << len
				  << " Error " << error_code.value() << std::endl;
}

void TCPConnection::handler_read(const boost::system::error_code &error_code,
								 size_t len) {
	if (error_code.value() == 0 && len > 0) {
		serverTCP_.parse_input(buffer_chat.data().data(), len);
	} else {
		serverTCP_.remove(shared_from_this());
		return;
	}
	buffer_chat.consume(len);
	async_read();
}

TCPConnection::pointer
TCPConnection::create(boost::asio::io_service &io_service,
					  ServerTCP &serverTCP) {
	return pointer(new TCPConnection(io_service, serverTCP));
}


tcp::socket &TCPConnection::socket() {
	return socket_;
}
