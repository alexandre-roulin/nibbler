#include <utility>

#include "ServerTCP.hpp"

namespace KNW {

	/** ServerTCP **/

	ServerTCP::ServerTCP(unsigned short port,
						 std::function<void(size_t)> callbackDeadSocket
) :
			connections( { nullptr } ),
			port_(port),
			acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
			dataTCP_(DataTCP::create()),
			callbackDeadSocket_(callbackDeadSocket) {

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
						[](boost::shared_ptr<ConnectionTCP> connection){
					return connection == nullptr || !connection->getSocket_().is_open();
				});

				*it = ConnectionTCP::create(*this, std::move(socket));
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
		log_fatal("%s", __PRETTY_FUNCTION__);
		connections.fill(nullptr);
		io_service_.stop();
		thread.join();
		thread.interrupt();
		acceptor_.cancel();
		acceptor_.close();
	}

	size_t ServerTCP::getSizeOfConnections() const {
		return std::count_if(connections.begin(), connections.end(),
						  [](boost::shared_ptr<ConnectionTCP> const &connection){
							  return connection != nullptr && connection->getSocket_().is_open();
						  });
	}

	void ServerTCP::callbackDeadConnection(size_t index) {
		connections[index] = nullptr;
		callbackDeadSocket_(index);
	}

	/** ConnectionTCP **/

	ConnectionTCP::ConnectionTCP(ServerTCP &serverTCP) :
			serverTCP_(serverTCP),
			iotcp(){

	}

	boost::shared_ptr<ConnectionTCP>
	ConnectionTCP::create(ServerTCP &serverTCP, tcp::socket socket) {
		auto sharedPtr = boost::shared_ptr<ConnectionTCP>(new ConnectionTCP(serverTCP));
		boost::weak_ptr<ConnectionTCP> weakPtr(sharedPtr);
		sharedPtr->iotcp = IOTCP::create(
				serverTCP.dataTCP_,
				std::move(socket),
				[weakPtr](){
					auto ptr = weakPtr.lock();
					if (ptr)
						ptr->callbackDeadIOTCP();
				});
		sharedPtr->iotcp->readSocketHeader();
		return sharedPtr;
	}


	void ConnectionTCP::sendData(std::string data) {
		iotcp->writeSocket(std::move(data));
		mutex.unlock();
	}

	ConnectionTCP::~ConnectionTCP() {
		log_fatal("%s",__PRETTY_FUNCTION__);
		iotcp = nullptr;
	}

	const tcp::socket &ConnectionTCP::getSocket_() const {
		return iotcp->getSocket_();
	}

	void ConnectionTCP::callbackDeadIOTCP() {
		log_warn("%s", __PRETTY_FUNCTION__ );
		auto it = std::find_if(serverTCP_.connections.begin(), serverTCP_.connections.end(),
				[this](boost::shared_ptr<ConnectionTCP> p){
					return (p.get()) == this;
		});

		if (it != serverTCP_.connections.end()) {
			serverTCP_.callbackDeadConnection(std::distance(serverTCP_.connections.begin(), it));
		}
	}


}

























