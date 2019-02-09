#include "ClientTCP.hpp"

namespace KNW {

	ClientTCP::ClientTCP(std::function<void()> callback)
			:
			socket_(io),
			resolver(io),
			dataTCP_(DataTCP::create()),
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
				dataTCP_->weak_from_this(),
				std::move(socket_),
				callbackDeadConnection_);

		iotcp->readSocketHeader();

		ClientTCP::boost_weak_ptr weakPtr(shared_from_this());
		thread = boost::thread([weakPtr](){
			if (auto ptr = weakPtr.lock()) ptr->io.run();
		});

		thread.detach();

	}

	bool ClientTCP::isConnect() const {
		return iotcp != nullptr && iotcp->isConnect();
	}

	ClientTCP::~ClientTCP() {
		log_success("%s", __PRETTY_FUNCTION__);
		if (iotcp != nullptr)
			iotcp.reset();
		if (socket_.is_open())
			socket_.close();
		if (thread.joinable()) {
			thread.join();
			thread.interrupt();
		}
		if (!io.stopped())
			io.reset();
	}

	void ClientTCP::disconnect() {
		if (iotcp != nullptr)
			iotcp.reset();
		if (thread.joinable()) {
			thread.join();
			thread.interrupt();
		}
	}

	ClientTCP::boost_shared_ptr ClientTCP::create(std::function<void()> f) {
		return KNW::ClientTCP::boost_shared_ptr(new ClientTCP(f));
	}

}