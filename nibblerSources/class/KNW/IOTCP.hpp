#pragma once

#include <boost/system/error_code.hpp>
#include "DataTCP.hpp"
#include <boost/bind.hpp>
#include "BaseDataType.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

namespace KNW {

	class ServerTCP;

	class IOTCP : public boost::enable_shared_from_this<KNW::IOTCP> {
	public:
		IOTCP() = delete;

		using boost_shared_ptr = boost::shared_ptr<IOTCP>;
		using boost_weak_ptr = boost::weak_ptr<IOTCP>;

		static boost::shared_ptr<KNW::IOTCP> create(
				DataTCP::boost_weak_ptr dataTCP_,
				tcp::socket socket_,
				std::function<void()> callbackDeadSocket
				);

		void writeSocket(std::string data);

		void readSocketHeader();

		const tcp::socket &getSocket_() const;

		virtual ~IOTCP();

		bool isConnect() const;
	private:
		IOTCP(
				DataTCP::boost_weak_ptr dataTCP_,
				tcp::socket socket_,
				std::function<void()> callbackDeadSocket
		);

		void checkError(boost::system::error_code const &error_code);

		void readSocketData(boost::weak_ptr<IOTCP> weakPtr, DataTCP::Header header);

		void handleReadHeader(boost::weak_ptr<IOTCP> weakPtr, const boost::system::error_code &ec, size_t len);


		void handleReadData(DataTCP::Header header,const boost::system::error_code &, size_t len);

		void handleWrite(const boost::system::error_code &, size_t len);

		DataTCP::boost_weak_ptr dataTCP_;
		boost::array<char, eConfigTCP::kMaxBufferSize> buffer_data_;
		tcp::socket socket_;
		std::function<void()> callbackDeadSocket_;
	};
}


