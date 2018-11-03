#include "ServerTCP.hpp"
#include "ClientTCP.hpp"

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
			new_connection->async_read();
			pointers.push_back(new_connection);
			refresh_data_snake_array(new_connection, nu_);
			refresh_data_snake(nu_);
			++nu_;
		}
		start_accept();
	});
}

void ServerTCP::refresh_data_snake(int16_t id) {
	unsigned int index;

	index = Snake::getSnakeById(snakes, MAX_SNAKE, id);

	assert(!(index == -1 || index > MAX_SNAKE));

	std::string buffer(reinterpret_cast<char *>(&id), sizeof(int16_t));
	buffer += std::string(reinterpret_cast<char *>(&this->snakes[index]), sizeof(Snake));
	ClientTCP::add_prefix(SNAKE, buffer);
	this->async_write(buffer);
}

void ServerTCP::refresh_data_snake_array(
		TCPConnection::pointer &connection,
		int16_t id) {
	{
		std::string buffer(reinterpret_cast<char *>(&id), sizeof(int16_t));
		ClientTCP::add_prefix(ID, buffer);
		connection->async_write(buffer);
	}
	{
		std::string buffer(reinterpret_cast<char *>(snakes), sizeof(Snake) * MAX_SNAKE);
		ClientTCP::add_prefix(SNAKE_ARRAY, buffer);
		connection->async_write(buffer);
	}
}

void ServerTCP::async_write(std::string message) {
	for (auto &pointer : pointers) {
		pointer->async_write(message);
	}
}

void ServerTCP::async_write(void *data, size_t bytes) {
	for (auto &pointer : pointers) {
		pointer->async_write(data, bytes);
	}
}

void ServerTCP::remove(TCPConnection::pointer remove) {
	pointers.erase(std::remove_if(pointers.begin(), pointers.end(),
								  [remove](TCPConnection::pointer pointer) {
									  std::cout << (remove == pointer)
												<< std::endl;
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

void TCPConnection::async_write(std::string message) {
	boost::asio::async_write(socket_, boost::asio::buffer(message),
							 boost::bind(&TCPConnection::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::async_write(void *data, size_t bytes) {
	boost::asio::async_write(socket_, boost::asio::buffer(data, bytes),
							 boost::bind(&TCPConnection::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::async_read() {
	boost::asio::async_read_until(socket_, streambuf_, '\n',
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
		serverTCP_.async_write(
				std::string(
						reinterpret_cast<char const *>(streambuf_.data().data()),
						len));
	} else {
		serverTCP_.remove(shared_from_this());
		std::cout << "Read : " << len
				  << " Error " << error_code.value() << std::endl;
		return;
	}
	streambuf_.consume(len);
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
