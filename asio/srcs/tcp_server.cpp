#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
	std::time_t now = std::time(nullptr);
	return std::ctime(&now);
}

int main() {
	try {
		boost::asio::io_service io;

		tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));

		for (;;) {
			tcp::socket socket(io);

			acceptor.accept(socket);

			std::string message = make_daytime_string();

			boost::system::error_code ign;
			boost::asio::write(socket, boost::asio::buffer(message), ign);
		}
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

}