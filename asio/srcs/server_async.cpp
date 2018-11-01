
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <network/ServerTCP.hpp>


using boost::asio::ip::tcp;

std::string make_daytime_string() {
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

class TCPConnection
		: public boost::enable_shared_from_this<TCPConnection> {
public:
	typedef boost::shared_ptr<TCPConnection> pointer;

	static pointer create(boost::asio::io_service &io_service) {
		return pointer(new TCPConnection(io_service));
	}

	tcp::socket &getSocket() {
		return socket_;
	}

	void start() {
		message_ = make_daytime_string();

		boost::asio::async_write(socket_, boost::asio::buffer(message_),
								 boost::bind(&TCPConnection::handle_write,
											 shared_from_this(),
											 boost::asio::placeholders::error,
											 boost::asio::placeholders::bytes_transferred));
	}

private:
	TCPConnection(boost::asio::io_service &io_service)
			: socket_(io_service) {
	}

	void handle_write(const boost::system::error_code & /*error*/,
					  size_t /*bytes_transferred*/) {
	}

	tcp::socket socket_;
	std::string message_;
};

class tcp_server {
public:
	tcp_server(boost::asio::io_service &io_service)
			: acceptor_(io_service, tcp::endpoint(tcp::v4(), 4242)) {
		start_accept();
	}

private:
	void start_accept() {
		TCPConnection::pointer new_connection =
				TCPConnection::create(acceptor_.get_io_service());

		acceptor_.async_accept(new_connection->getSocket(),
							   boost::bind(&tcp_server::handle_accept, this,
										   new_connection,
										   boost::asio::placeholders::error));
	}

	void handle_accept(TCPConnection::pointer new_connection,
					   const boost::system::error_code &error) {
		if (!error) {
			new_connection->start();
		}

		start_accept();
	}

	tcp::acceptor acceptor_;
};

int main() {
	try {
		boost::asio::io_service io_service;
		tcp_server server(io_service);
		io_service.run();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

