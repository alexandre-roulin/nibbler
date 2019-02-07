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
#include "ClientTCP.hpp"

namespace KNW {
	/*********************** ClientTCP ************************************/

	class ClientTCP {
	public:

		ClientTCP(std::function<void()>);

		void connect(std::string dns, std::string port);

		template<typename T>
		void addDataType(std::function<void(T)> callback);

		template<typename T, typename H>
		void addDataType(std::function<void(T)> callback, H);
		template<typename T>
		void writeDataToServer(T data);
		template<typename T>
		void writeDataToServer(T data, uint16_t);

		virtual ~ClientTCP();

		bool isConnect() const;

		void disconnect();
	private:

		boost::asio::io_service io;
		tcp::socket socket_;
		tcp::resolver resolver;
		boost::thread thread;
		DataTCP dataTCP_;
		boost::shared_ptr<IOTCP> iotcp;
		std::function<void()> callbackDeadConnection_;
	};

	template<typename T>
	void ClientTCP::addDataType(std::function<void(T)> callback) {
		assert(!dataTCP_.hasType<T>());
		dataTCP_.addDataType<T>(callback);
	}

	template<typename T>
	void ClientTCP::writeDataToServer(T data) {
		assert(dataTCP_.hasType<T>());
		auto header = DataType<T>::getHeader();
//		std::cout << "writeDataToServer:" << dataTCP_.getSizeOfType<T>() << std::endl;
		std::string buffer;
		buffer.append(reinterpret_cast<char *>(&header), sizeof(BaseDataType::Header));
		buffer.append(reinterpret_cast<char *>(&data), dataTCP_.getSizeOfHeader(header));
		iotcp->writeSocket(buffer);
	}

	template<typename T>
	void ClientTCP::writeDataToServer(T data, uint16_t header) {
		std::string buffer;
		buffer.append(reinterpret_cast<char *>(&header), sizeof(uint16_t));
		buffer.append(reinterpret_cast<char *>(&data), dataTCP_.getSizeOfHeader(header));
		iotcp->writeSocket(buffer);
	}

	template<typename T, typename H>
	void ClientTCP::addDataType(std::function<void(T)> callback, H index) {
		dataTCP_.addDataType<T, H>(callback, index);
	}


}


