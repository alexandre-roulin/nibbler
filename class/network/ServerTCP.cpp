#include "ServerTCP.hpp"

ServerTCP::ServerTCP(boost::asio::io_service &io_service)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), 4242)),
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

			std::cout << "ServerTCP::new_connection > "
					  << socket.native_handle() << std::endl;
			new_connection->async_read();
			pointers.push_back(new_connection);
		}
		start_accept();
	});
}

void ServerTCP::async_write(std::string message) {
	for (auto &pointer : pointers) {
		pointer->async_write(message);
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
	if (error_code != 0)
		std::cout << "Write : " << len
				  << " Error " << error_code.value() << std::endl;
}

void TCPConnection::handler_read(const boost::system::error_code &error_code,
								 size_t len) {

	if (error_code == 0 && len > 0) {
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

