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

void ServerTCP::parse_input(eHeader header, void const *input, size_t len) {

	size_t real_len = (header == CHAT) ? len - 1 : len;
	char *data_deserialize = new char[len];

	std::memcpy(data_deserialize, reinterpret_cast<char const *>(input), real_len);

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
	std::string buffer;
	ClientTCP::add_prefix(header, buffer, data_deserialize, real_len);
	std::cout << "ServerTCP::parse_input.size() " << buffer.size() << std::endl;
	async_write(buffer);
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


void
TCPConnection::handle_read_data(boost::system::error_code const &error_code,
								size_t len) {
	std::cout << "TCPConnection::handle_read_data [" << len << "]" << std::endl;

	if (error_code.value() == 0 && len > 0) {
		eHeader header = static_cast<eHeader >(len);
		serverTCP_.parse_input(header, buffer_data.data(), len);
	}
	buffer_chat.consume(len);
	read_socket_header();
}

void
TCPConnection::handle_read_chat(boost::system::error_code const &error_code,
								size_t len) {
	std::cout << "TCPConnection::handle_read_chat" << std::endl;
	std::cerr << error_code << " " << len << std::endl;

	if (error_code.value() == 0 && len > 0) {
		serverTCP_.parse_input(CHAT, buffer_chat.data().data(), len);
	}
	buffer_chat.consume(len);
	read_socket_header();
}


void TCPConnection::handle_write(const boost::system::error_code &error_code,
								 size_t len) {
}


void TCPConnection::read_socket_header() {

	std::cout << "TCPConnection::read_socket_header" << std::endl;
	boost::asio::async_read(socket_,
							boost::asio::buffer(buffer_data, sizeof(eHeader)),
							boost::bind(&TCPConnection::handle_read_header,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::read_socket_data(eHeader header) {
	std::cout << "TCPConnection::read_socket_data" << std::endl;
	boost::asio::async_read(socket_, boost::asio::buffer(buffer_data, header),
							boost::bind(&TCPConnection::handle_read_data,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::read_socket_chat() {
	std::cout << "TCPConnection::read_socket_chat" << std::endl;
	boost::asio::async_read_until(socket_, buffer_chat, '\n',
								  boost::bind(&TCPConnection::handle_read_chat,
											  shared_from_this(),
											  boost::asio::placeholders::error,
											  boost::asio::placeholders::bytes_transferred));
}

void
TCPConnection::handle_read_header(const boost::system::error_code &error_code,
								  size_t len) {
	std::cout << "TCPConnection::handle_read_header > len : " << len
			  << std::endl;
	if (error_code.value() == 0) {
		assert(len == sizeof(eHeader));

		eHeader header;
		std::memcpy(&header, buffer_data.data(), sizeof(eHeader));

		std::cout << "Header : " << header << std::endl;
		switch (header) {
			case CHAT:
				read_socket_chat();
				break;
			case SNAKE_ARRAY:
				read_socket_data(SNAKE_ARRAY);
				break;
			case SNAKE:
				read_socket_data(SNAKE);
				break;
			case FOOD:
				break;
			case ID:
				read_socket_data(ID);
				break;
		}
	}
}

void TCPConnection::write_socket(void const *data, size_t len) {
	boost::asio::async_write(socket_, boost::asio::buffer(data, len),
							 boost::bind(&TCPConnection::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::write_socket(std::string message) {
	boost::asio::async_write(socket_, boost::asio::buffer(message),
							 boost::bind(&TCPConnection::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}


TCPConnection::pointer
TCPConnection::create(boost::asio::io_service &io_service,
					  ServerTCP &serverTCP) {
	return pointer(new TCPConnection(io_service, serverTCP));
}


tcp::socket &TCPConnection::socket() {
	return socket_;
}
