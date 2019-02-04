#include <utility>

#include "ServerTCP.hpp"

namespace KNW {

	/** ServerTCP **/

	ServerTCP::ServerTCP(unsigned short port) :
			port_(port),
			acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
			connections( { nullptr } ){
	}

	void ServerTCP::accept() {
		asyncAccept();
		thread = boost::thread(
				boost::bind(&boost::asio::io_service::run, &io_service_));
		thread.detach();
	}


	void ServerTCP::accept(std::function<void(size_t)> callbackAccept) {
		callbackAccept_ = callbackAccept;
		asyncAccept();
		thread = boost::thread(
				boost::bind(&boost::asio::io_service::run, &io_service_));
		thread.detach();
	}


	void ServerTCP::asyncAccept() {
		acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
			if (ec.value() == 0) {
				auto it = std::find_if(connections.begin(), connections.end(),
						[](std::shared_ptr<ConnectionTCP> connection){
							return connection == nullptr || !connection->getSocket_().is_open();
				});
				*it = std::make_shared<ConnectionTCP>(*this, std::move(socket));
				std::cout << "New connection" << std::endl;
				if (callbackAccept_) {
					std::cout << "Callback" << std::endl;
					callbackAccept_(std::distance(connections.begin(), std::find(connections.begin(), connections.end(), *it)));
				}
			} else {
				std::cout << ec.message() << std::endl;
			}
			asyncAccept();
		});
	}

	ServerTCP::~ServerTCP() {
		io_service_.stop();
		acceptor_.cancel();
		acceptor_.close();
		thread.interrupt();
	}

	size_t ServerTCP::getSizeOfConnections() const {
		return std::count_if(connections.begin(), connections.end(),
						  [](std::shared_ptr<ConnectionTCP> const &connection){
							  return connection != nullptr && connection->getSocket_().is_open();
						  });
	}

	/** ConnectionTCP **/

	ConnectionTCP::ConnectionTCP(ServerTCP &serverTCP, tcp::socket socket) :
			iotcp(std::make_unique<IOTCP>(
					serverTCP.dataTCP_,
					std::move(socket),
					std::bind(&DataTCP::sendDataToCallback,
							  std::ref(serverTCP.dataTCP_),
							  std::placeholders::_1,
							  std::placeholders::_2
					))) {

		iotcp->readSocketHeader();
	}

	void ConnectionTCP::sendData(std::string data) {
		iotcp->writeSocket(std::move(data));
	}

	ConnectionTCP::~ConnectionTCP() {
		std::cout << "~ConnectionTCP" << std::endl;
	}

	const tcp::socket &ConnectionTCP::getSocket_() const {
		return iotcp->getSocket_();
	}

}