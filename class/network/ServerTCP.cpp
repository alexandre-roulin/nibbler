#include "ServerTCP.hpp"
#include <logger.h>

ServerTCP::ServerTCP(boost::asio::io_service &io_service)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), 4242)) {
	boost::thread t(boost::bind(&ServerTCP::start_accept, this));
	t.detach();
//	start_accept();
}

void ServerTCP::start_accept() {
	std::cout << "start_accept" << std::endl;

	TCPConnection::pointer new_connection =
			TCPConnection::create(acceptor_.get_io_service());
	acceptor_.accept(new_connection->getSocket());
	if (new_connection->getSocket().is_open()) {
		new_connection->start();
		pointers.push_back(new_connection);
	}
	std::cout << "start_accept::end" << std::endl;
	start_accept();
}

int ServerTCP::size_pointers() {
	return pointers.size();
}


void TCPConnection::start() {
	std::string message_("Bonjour a vous !");
	boost::asio::async_write(socket_, boost::asio::buffer(message_),
							 boost::bind(&TCPConnection::handle_write,
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

