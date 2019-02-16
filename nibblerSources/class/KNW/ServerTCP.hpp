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

		boost::asio::ip::tcp::socket &getSocket_();

		void write(std::string data);

		static b_sptr create(
				boost::weak_ptr<ServerTCP> serverTCP,
				boost::shared_ptr<boost::asio::ip::tcp::socket> sock
		);

		virtual ~ConnectionTCP();

	private:
		friend class ServerTCP;

		ConnectionTCP();

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
		// typedef

		using b_sptr = boost::shared_ptr<ServerTCP>;
		using b_wptr = boost::weak_ptr<ServerTCP>;

		//static creator

		static b_sptr create(IOManager &io_manager);

		//member function

		DataTCP &getDataTCP();

		void startServer(uint16_t port) noexcept;


		size_t getSizeOfOpenConnection() const;

		void startAsyncAccept() noexcept;

		void startAsyncAccept(std::function<void(size_t)>) noexcept;

		void startAsyncAccept(std::function<void(size_t)>,
							  std::function<void(size_t)>) noexcept;

		template<typename T>
		void writeDataToOpenConnection(T data, int index);

		template<typename T, typename H>
		void writeDataToOpenConnection(T data, int index, H header);

		template<typename T>
		void writeDataToOpenConnections(T data);

		template<typename T, typename H>
		void writeDataToOpenConnections(T data, H header);

		void stopAccept();

		virtual ~ServerTCP();

	private:

		explicit ServerTCP(IOManager &io_manager);

		void acceptConnection() noexcept;

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
		boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
	};

	template<typename T>
	void ServerTCP::writeDataToOpenConnection(T data, int index) {
		assert(dataTCP->hasType<T>());
		assert(connections[index] != nullptr);
		connections[index]->write(dataTCP->serializeData(DataType<T>::getHeader(), data));
	}

	template<typename T, typename H>
	void ServerTCP::writeDataToOpenConnection(T data, int index, H header) {
		assert(connections[index] != nullptr);
		connections[index]->write(dataTCP->serializeData(static_cast<BaseDataType::Header>(header), data));
	}

	template<typename T>
	void ServerTCP::writeDataToOpenConnections(T data) {
		assert(dataTCP->hasType<T>());
		for (auto &connection : connections) {
			if (connection) {
				connection->write(dataTCP->serializeData(DataType<T>::getHeader(), data));
			}
		}
	}

	template<typename T, typename H>
	void ServerTCP::writeDataToOpenConnections(T data, H header) {
		for (auto &connection : connections) {
			if (connection)
				connection->write(dataTCP->serializeData(static_cast<BaseDataType::Header>(header), data));
		}
	}

}
