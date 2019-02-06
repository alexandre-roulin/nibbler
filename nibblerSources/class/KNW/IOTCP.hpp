#pragma once

#include <boost/system/error_code.hpp>
#include "DataTCP.hpp"
#include <boost/bind.hpp>
#include "BaseDataType.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>


using boost::asio::ip::tcp;

namespace KNW {

	class ServerTCP;

	class IOTCP {
	public:
		IOTCP(
				DataTCP &dataTCP_,
				tcp::socket socket_,
				std::function<void(BaseDataType::Header, char *)> f,
				std::function<void()> callbackDeadSocket
		);

		void writeSocket(std::string data);

		void readSocketHeader();

		const tcp::socket &getSocket_() const;

		virtual ~IOTCP();

		bool isConnect() const;
	private:
		void checkError(boost::system::error_code const &error_code);

		void readSocketData(DataTCP::Header header);

		void handleReadHeader(const boost::system::error_code &, size_t);


		void handleReadData(DataTCP::Header header,const boost::system::error_code &);

		void handleWrite(const boost::system::error_code &);

		DataTCP &dataTCP_;
		boost::array<char, eConfigTCP::kMaxBufferSize> buffer_data_;
		tcp::socket socket_;
		std::function<void(BaseDataType::Header, char *)> callback_;
		std::function<void()> callbackDeadSocket_;
	};
}


