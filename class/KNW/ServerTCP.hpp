#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/error.hpp>
#include <boost/array.hpp>
#include <boost/asio/detail/config.hpp>
#include <boost/cerrno.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <array>
#include "BaseDataType.hpp"
#include <bitset>
#include <unordered_map>
#include <iostream>
#include "Config.hpp"
#include "DataTCP.hpp"
#include "IOTCP.hpp"


using boost::asio::ip::tcp;

namespace KNW {

	class ServerTCP;

	/*********************** ConnectionTCP ************************************/

	class ConnectionTCP {
	public:
		ConnectionTCP(ServerTCP &serverTCP, tcp::socket socket);
		void sendData(std::string data);
		~ConnectionTCP();
	private:
		friend class ServerTCP;
		std::unique_ptr<IOTCP> iotcp;
		ServerTCP &serverTCP_;
	};

	/*************************** ServerTCP ************************************/

	class ServerTCP {
	public:
		explicit ServerTCP(unsigned short port);

		size_t getSizeOfConnections() const;

		void accept();

		void accept(std::function<void(size_t)>);

		template<typename T>
		void addDataType(std::function<void(T)> callback);

		template<typename T, typename H>
		void addDataType(std::function<void(T)> callback, H);

		template<typename T>
		void writeDataToOpenConnections(T data);

		template<typename T>
		void writeDataToOpenConnection(T data, int index);

		template<typename T, typename H>
		void writeDataToOpenConnection(T data, int index, H header);

		template<typename T, typename H>
		void writeDataToOpenConnections(T data, H header);

		virtual ~ServerTCP();
	private:

		void asyncAccept();

		//Network
		unsigned short port_;
		boost::asio::io_service io_service_;
		tcp::acceptor acceptor_;
		boost::thread thread;

		//connection
		std::vector<std::shared_ptr<ConnectionTCP>> connections;
		std::function<void(size_t)> callbackAccept_;
		//Data management
		DataTCP dataTCP_;
		friend class ConnectionTCP;
		friend class IOTCP;

	};

	template<typename T>
	void ServerTCP::addDataType(std::function<void(T)> callback) {
		assert(!dataTCP_.hasType<T>());
		dataTCP_.addDataType<T>(callback);
	}

	template<typename T, typename H>
	void ServerTCP::addDataType(std::function<void(T)> callback, H index) {
		dataTCP_.addDataType<T, H>(callback, index);
	}


	template<typename T>
	void ServerTCP::writeDataToOpenConnections(T data) {
		assert(dataTCP_.hasType<T>());

		auto header = DataType<T>::getHeader();

		std::string buffer;
		buffer.append(reinterpret_cast<char *>(&header), sizeof(BaseDataType::Header));
		buffer.append(reinterpret_cast<char *>(&data), dataTCP_.getSizeOfHeader(header));

		for (auto &connection : connections) {
			connection->sendData(buffer);
		}
	}

	template<typename T, typename H>
	void ServerTCP::writeDataToOpenConnections(T data, H header) {
		uint16_t header_ = static_cast<uint16_t >(header);
		std::string buffer;
		buffer.append(reinterpret_cast<char *>(&header_), sizeof(BaseDataType::Header));
		buffer.append(reinterpret_cast<char *>(&data), dataTCP_.getSizeOfHeader(header_));

		for (auto &connection : connections) {
			connection->sendData(buffer);
		}
	}


	template<typename T>
	void ServerTCP::writeDataToOpenConnection(T data, int index) {
		assert(index < connections.size());

		auto header = DataType<T>::getHeader();
		std::string buffer;
		buffer.append(reinterpret_cast<char *>(&header), sizeof(BaseDataType::Header));
		buffer.append(reinterpret_cast<char *>(&data), dataTCP_.getSizeOfHeader(header));

		connections[index]->sendData(buffer);
	}

	template<typename T, typename H>
	void
	ServerTCP::writeDataToOpenConnection(T data, int index, H header) {
		uint16_t header_ = static_cast<uint16_t >(header);
		assert(index < connections.size());
//		log_success("%s(%d)", __PRETTY_FUNCTION__, index);
		std::string buffer;
		buffer.append(reinterpret_cast<char *>(&header_), sizeof(BaseDataType::Header));
		buffer.append(reinterpret_cast<char *>(&data), dataTCP_.getSizeOfHeader(header_));
		connections[index]->sendData(buffer);
	}

}


