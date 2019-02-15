#include "ClientTCP.hpp"


namespace KNW {

	ClientTCP::ClientTCP(IOManager &io_manager)
			:
			dataTCP_(DataTCP::create()),
			io_manager_(io_manager) {

	}

	ClientTCP::b_sptr ClientTCP::create(IOManager &io_manager,std::function<void()> c) {
		auto ptr = boost::shared_ptr<ClientTCP>(new ClientTCP(io_manager));
		ptr->callbackDeadConnection_ = std::move(c);
		return ptr;
	}

	void ClientTCP::connect(const std::string host, const std::string port) {
		boost::asio::ip::tcp::resolver resolver(io_manager_.getIo());
		boost::shared_ptr<boost::asio::ip::tcp::socket> socket_(new boost::asio::ip::tcp::socket(io_manager_.getIo()));

		boost::asio::ip::tcp::resolver::query query(
				host,
				boost::lexical_cast<std::string>(port));

		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(
				query);
		ClientTCP::b_wptr wptr(shared_from_this());
		boost::system::error_code ec;
		socket_->connect(*iterator, ec);
		if (!ec) {
			iotcp = IOTCP::create(dataTCP_, socket_, callbackDeadConnection_);
		} else {
			std::cout << ec.message() << std::endl;
		}
	}

	bool ClientTCP::isConnect() const {
		return iotcp != nullptr && iotcp->isConnect();
	}

	DataTCP &ClientTCP::getDataTCP_() {
		return *dataTCP_;
	}


	void ClientTCP::disconnect() {
		if (iotcp) {
			boost::system::error_code ec_sock;
			iotcp->getSocket_().shutdown(
					boost::asio::ip::tcp::socket::shutdown_both, ec_sock);
			iotcp->getSocket_().close(ec_sock);
		}
		iotcp = nullptr;
	}

	ClientTCP::~ClientTCP() {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		disconnect();
	}


}























