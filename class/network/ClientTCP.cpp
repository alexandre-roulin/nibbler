#include "ClientTCP.hpp"

ClientTCP::ClientTCP(boost::asio::io_service &io, std::string &hostname)
		: resolver(tcp::resolver(io)),
		  query(tcp::resolver::query(hostname, "4242")),
		  socket(tcp::socket(io)),
		  timer(io, boost::posix_time::seconds(1)) {
	timer.async_wait(boost::bind(&ClientTCP::read_socket, this));
	std::cout << "Trying to connect" << std::endl;
	tcp::resolver::iterator it = resolver.resolve(query);
	boost::asio::connect(socket, it);
	std::cout << "Connection successfull" << std::endl;
}

void ClientTCP::read_socket() {
	std::cout  << std::endl<< "Read !" << std::endl;
	boost::array<char, 128> buff{};
	boost::system::error_code error;
	size_t len = socket.read_some(boost::asio::buffer(buff), error);

	if (error == boost::asio::error::eof)
		return;
	else if (error)
		throw boost::system::system_error(error);
	std::cout.write(buff.data(), len);
	timer.expires_at(timer.expires_at() + boost::posix_time::seconds(1));
	timer.async_wait(boost::bind(&ClientTCP::read_socket, this));
}
