#include <utility>

#include "ServerTCP.hpp"

namespace KNW {

	/** ServerTCP **/

	ServerTCP::ServerTCP(unsigned short port,
						 std::function<void(size_t)> callbackDeadSocket
) :
			connections( { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr } ),
			port_(port),
			acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
			dataTCP_(DataTCP::create()),
			callbackDeadSocket_(callbackDeadSocket) {

	}

	void ServerTCP::accept() {
		boost_weak_ptr weakPtr(shared_from_this());
		asyncAccept(weakPtr);
		thread = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
		thread.detach();
	}


	void ServerTCP::accept(std::function<void(size_t)> callbackAccept) {
		callbackAccept_ = callbackAccept;
		boost_weak_ptr weakPtr(shared_from_this());
		asyncAccept(weakPtr);
		thread = boost::thread(
				boost::bind(&boost::asio::io_service::run, &io_service_));
		thread.detach();
	}


	void ServerTCP::asyncAccept(boost_weak_ptr weakPtr) {

		acceptor_.async_accept([weakPtr](std::error_code ec, tcp::socket socket) {
			auto ptr = weakPtr.lock();
			if (ptr) {
				if (ec.value() == 0) {

					auto it = std::find_if(ptr->connections.begin(), ptr->connections.end(),
										   [](boost::shared_ptr<ConnectionTCP> connection){
											   return connection == nullptr || !connection->getSocket_().is_open();
										   });
					if (it != ptr->connections.end()) {
						*it = ConnectionTCP::create(ptr, std::move(socket));
						if (ptr->callbackAccept_) {
							ptr->callbackAccept_(std::distance(ptr->connections.begin(), std::find(ptr->connections.begin(), ptr->connections.end(), *it)));
						}
					} else {
						auto e = boost::system::errc::make_error_code(boost::system::errc::too_many_files_open);
						socket.close(e);
					}
					ptr->io_service_.post([weakPtr]() { if (auto ptrServer = weakPtr.lock()) ptrServer->asyncAccept(ptrServer); });
				} else {
					std::cout << ec.message() << std::endl;
				}
			}
		});
	}

	ServerTCP::~ServerTCP() {
		callbackAccept_ = nullptr;
		callbackDeadSocket_ = nullptr;
		dataTCP_.reset();
		log_fatal("%s", __PRETTY_FUNCTION__);
		io_service_.stop();
		acceptor_.cancel();
		thread.join();
		thread.interrupt();
		connections.fill(nullptr);
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

	ServerTCP::boost_shared_ptr ServerTCP::create(unsigned short port,
												  std::function<void(
														  size_t)> callbackDeadSocket) {
		return ServerTCP::boost_shared_ptr(new ServerTCP(port, std::move(callbackDeadSocket)));
	}

	void ServerTCP::runIO() {

	}

	/** ConnectionTCP **/

	ConnectionTCP::ConnectionTCP(boost::weak_ptr<ServerTCP> weakServerPtr) :
			weakServerPtr_(weakServerPtr),
			iotcp(){

	}

	boost::shared_ptr<ConnectionTCP>
	ConnectionTCP::create(boost::weak_ptr<ServerTCP> weakServerPtr, tcp::socket socket) {
		auto sharedPtr = boost::shared_ptr<ConnectionTCP>(new ConnectionTCP(weakServerPtr));
		boost::weak_ptr<ConnectionTCP> weakPtr(sharedPtr);

		if (auto ptrServer = weakServerPtr.lock()) {
			sharedPtr->iotcp = IOTCP::create(
					ptrServer->dataTCP_,
					std::move(socket),
					[weakPtr](){
						auto ptr = weakPtr.lock();
						if (ptr)
							ptr->callbackDeadIOTCP();
					});
			sharedPtr->iotcp->readSocketHeader();
		}
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
		if (auto serverPtr = weakServerPtr_.lock()) {
			ConnectionTCP::boost_weak_ptr weakPtr(shared_from_this());
			if (auto ptr = weakPtr.lock()) {
				auto it = std::find_if(serverPtr->connections.begin(), serverPtr->connections.end(),
									   [ptr](boost::shared_ptr<ConnectionTCP> p){
										   return p == ptr;
									   });

				if (it != serverPtr->connections.end()) {
					serverPtr->callbackDeadConnection(std::distance(serverPtr->connections.begin(), it));
				}
			}
		}
	}
}

























