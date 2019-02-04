#include "ClientTCP.hpp"

namespace KNW {

	ClientTCP::ClientTCP()
			: socket_(io),  resolver(io)
			  {
		std::cout << "ClientTCP" << std::endl;
	}

	void ClientTCP::connect(std::string dns, std::string port) {
		std::cout << "socket : " << socket_.is_open() << std::endl;
		try {
			tcp::resolver::query query(dns, port);
			tcp::resolver::iterator it = resolver.resolve(query);
			boost::asio::connect(socket_, it);
			iotcp = std::make_unique<IOTCP>(
					dataTCP_,
					std::move(socket_),
					std::bind(&DataTCP::sendDataToCallback, std::ref(dataTCP_),
							  std::placeholders::_1, std::placeholders::_2));
			iotcp->readSocketHeader();
			thread = boost::thread(
					boost::bind(&boost::asio::io_service::run, &io));
			thread.detach();
		} catch (std::exception &exception) {
			std::cout << exception.what() << std::endl;
		}

	}

	bool ClientTCP::isConnect() const {
		return socket_.is_open();
	}

	ClientTCP::~ClientTCP() {
		log_fatal("%s", __PRETTY_FUNCTION__);
		try {
			if (socket_.is_open()) {
				socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
				socket_.close();
			}
		} catch (std::exception const &e) {
			std::cout << e.what() << std::endl;
		}

		thread.interrupt();
		io.stop();
	}

}