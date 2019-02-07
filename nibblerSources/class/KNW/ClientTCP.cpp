#include "ClientTCP.hpp"

namespace KNW {

	ClientTCP::ClientTCP(std::function<void()> callback)
			:
			socket_(io),
			resolver(io),
			callbackDeadConnection_(callback)

			{
		std::cout << "ClientTCP" << std::endl;
	}

	void ClientTCP::connect(std::string dns, std::string port) {
		std::cout << "socket : " << socket_.is_open() << std::endl;
		tcp::resolver::query query(dns, port);
		tcp::resolver::iterator it = resolver.resolve(query);
		boost::asio::connect(socket_, it);
		iotcp =  IOTCP::create(
				dataTCP_,
				std::move(socket_),
				std::bind(
						&DataTCP::sendDataToCallback,
						std::ref(dataTCP_),
						std::placeholders::_1,
						std::placeholders::_2),
				callbackDeadConnection_);


		iotcp->readSocketHeader();
		thread = boost::thread(boost::bind(&boost::asio::io_service::run, &io));
		thread.detach();

	}

	bool ClientTCP::isConnect() const {
		return iotcp != nullptr && iotcp->isConnect();
	}

	ClientTCP::~ClientTCP() {
		log_fatal("%s",__PRETTY_FUNCTION__);
		int i = 0;
		log_fatal("%d", ++i);
		thread.join();
		log_fatal("%d", ++i);
		log_fatal("%d", ++i);
		thread.interrupt();
		iotcp = nullptr;
		log_fatal("%d", ++i);
		io.reset();
		log_fatal("%d", ++i);
		io.stop();
		log_fatal("%d", ++i);
	}

	void ClientTCP::disconnect() {
		iotcp = nullptr;
	}

}