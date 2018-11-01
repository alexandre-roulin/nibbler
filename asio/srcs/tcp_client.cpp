#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int ac, char *av[]) {

	try {
		if (ac != 2) {
			std::cerr << "usage : client <host>" << std::endl;
			exit(1);
		}
		boost::asio::io_service io;
		tcp::resolver resolver(io);

		std::cout << av[1] << std::endl;
		tcp::resolver::query query(av[1], "4242");

		tcp::resolver::iterator it = resolver.resolve(query);

		tcp::socket socket(io);
		boost::asio::connect(socket, it);

		for (;;) {
			boost::array<char, 128> buff{};
			boost::system::error_code error;
			size_t len = socket.read_some(boost::asio::buffer(buff), error);
			if (error == boost::asio::error::eof)
				break;
			else if (error)
				throw boost::system::system_error(error);
			std::cout.write(buff.data(), len);
		}

	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}