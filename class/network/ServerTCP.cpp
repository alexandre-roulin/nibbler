#include "ServerTCP.hpp"
#include <boost/array.hpp>

ServerTCP::ServerTCP(boost::asio::io_service &io_service)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), 4242)),
		  timer_accept(io_service, boost::posix_time::seconds(1)) {
	std::cout << "Server created" << std::endl;
	start_accept();
}


void ServerTCP::start_accept() {
	std::cout << "start_accept" << std::endl;
	acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
		std::cout << "async_accept::find" << std::endl;
		if (!ec) {
			TCPConnection::pointer new_connection =
					TCPConnection::create(acceptor_.get_io_service());
			new_connection->socket_ = std::move(socket);
			new_connection->async_read();
			pointers.push_back(new_connection);
		}
		start_accept();
	});
}

int ServerTCP::size_pointers() {
	return pointers.size();
}

void TCPConnection::handler_read(const boost::system::error_code &error_code,
								 size_t len) {
	if (error_code == 0) {
		std::cout << static_cast<char const *>(b.data().data());
	}
	b.consume(b.size());
	async_read();
}

void
TCPConnection::handle_write(const boost::system::error_code &e, size_t len) {
	std::cout << "Write : " << len << " Error " << e.value() << std::endl;
}

void ServerTCP::hello() {
	for (auto &pointer : pointers) {
		pointer->async_write("salut tout le monde\n");
	}
}

/** ---------------------- TCPConnection ---------------------- **/


void TCPConnection::async_write(std::string message) {
	boost::asio::async_write(socket_, boost::asio::buffer(message),
							 boost::bind(&TCPConnection::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::async_read() {
	std::cout << "TCPConnection::async_read " << socket_.is_open() << std::endl;
	boost::asio::async_read_until(socket_, b, '\n',
								  boost::bind(&TCPConnection::handler_read,
											  shared_from_this(),
											  boost::asio::placeholders::error,
											  boost::asio::placeholders::bytes_transferred));
}

TCPConnection::pointer
TCPConnection::create(boost::asio::io_service &io_service) {
	return pointer(new TCPConnection(io_service));
}

tcp::socket &TCPConnection::getSocket() {
	return socket_;
}

