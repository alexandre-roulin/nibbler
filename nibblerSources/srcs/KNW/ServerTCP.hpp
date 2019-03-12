#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "IOManager.hpp"
#include "IOTCP.hpp"
#include "Config.hpp"

namespace KNW {


	/** Connection TCP **/
	class ServerTCP;

	class ConnectionTCP : public boost::enable_shared_from_this<ConnectionTCP> {
	public:
		using b_sptr = boost::shared_ptr<ConnectionTCP>;

		virtual ~ConnectionTCP();
		ConnectionTCP &operator=(const ConnectionTCP &) = delete;
		ConnectionTCP(const ConnectionTCP &) = delete;

		const boost::shared_ptr<boost::asio::ip::tcp::socket> &getSocket_();

		void write(std::string data);
		void write(const void *pVoid, size_t len);

		static b_sptr create(
				boost::weak_ptr<ServerTCP> serverTCP,
				boost::shared_ptr<boost::asio::ip::tcp::socket> sock
		);


	private:
		friend class ServerTCP;

		ConnectionTCP();
		bool isOpen() const;
		void callbackDeadIO();

		boost::weak_ptr<ServerTCP> serverTCP_;
		IOTCP::b_sptr iotcp;
	};


	class ServerTCP : public boost::enable_shared_from_this<ServerTCP> {

		/** Server TCP **/
	public:
		ServerTCP() = delete;
		ServerTCP(ServerTCP &) = delete;
		ServerTCP(ServerTCP const &) = delete;
		ServerTCP &operator=(const ServerTCP &) = delete;

		// typedef

		using b_sptr = boost::shared_ptr<ServerTCP>;
		using b_wptr = boost::weak_ptr<ServerTCP>;

		//static creator

		static b_sptr create(IOManager &io_manager);

		//member function

		DataTCP::b_sptr getDataTCP();
		bool hasDataTCP() const;

		void startServer(const std::string dns, uint16_t port);

		unsigned short getPort() const;

		std::string const &getAddress() const;

		size_t getSizeOfOpenConnection() const;

		void startAsyncAccept();

		void startAsyncAccept(std::function<void(size_t)>);

		void startAsyncAccept(std::function<void(size_t)>,
							  std::function<void(size_t)>);

		void registerAcceptCallback(std::function<void(size_t)>);
		void registerDeadConnectionCallback(std::function<void(size_t)>);

		template<typename T>
		void writeDataToOpenConnection(T &&data, int index);

		template<typename T, typename H>
		void writeDataToOpenConnection(T &&data, int index, H header);

		template<typename T>
		void writeDataToOpenConnections(T &&data);

		template<typename T, typename H>
		void writeDataToOpenConnections(T &&data, H header);

		void stopAccept();

		bool isOpen() const;

		virtual ~ServerTCP();

	private:

		explicit ServerTCP(IOManager &io_manager);

		void acceptConnection();

		std::array<boost::shared_ptr<ConnectionTCP>, eConfigTCP::kMaxConnectionOpen> connections;

		void handleAcceptConnection(
				const boost::system::error_code &ec,
				boost::shared_ptr<boost::asio::ip::tcp::socket> sock);

		friend class ConnectionTCP;

		IOManager &io_manager_;
		DataTCP::b_sptr dataTCP;
		std::function<void(size_t)> callbackAccept_;
		std::function<void(size_t)> callbackDeadConnection_;
	private:
		unsigned short port_;
		std::string address_;
		boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
		bool open;
	};

	template<typename T>
	void ServerTCP::writeDataToOpenConnection(T &&data, int index) {
		if (index < 0 && index >= eConfigTCP::kMaxConnectionOpen)
			return;
		assert(dataTCP->hasType<T>());
		if (connections[index] && connections[index]->isOpen())
			connections[index]->write(dataTCP->serializeData(DataType<T>::getHeader(), data));
	}

	template<typename T, typename H>
	void ServerTCP::writeDataToOpenConnection(T &&data, int index, H header) {
		if (index < 0 && index >= eConfigTCP::kMaxConnectionOpen &&
		connections[index] != nullptr)
			return;
		if (connections[index] && connections[index]->isOpen())
			connections[index]->write(dataTCP->serializeData(static_cast<BaseDataType::Header>(header), data));
	}

	template<typename T>
	void ServerTCP::writeDataToOpenConnections(T &&data) {
		assert(dataTCP->hasType<T>());
		for (auto &connection : connections) {
			if (connection && connection->isOpen()) {
				connection->write(dataTCP->serializeData(DataType<T>::getHeader(), data));
			}
		}
	}

	template<typename T, typename H>
	void ServerTCP::writeDataToOpenConnections(T &&data, H header) {

		for (auto &connection : connections) {
			if (connection && connection->isOpen() && dataTCP) {
				connection->write(dataTCP->serializeData(static_cast<BaseDataType::Header>(header), data));
			}
		}
	}

}
