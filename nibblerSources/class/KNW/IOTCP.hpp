#pragma once

#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "DataTCP.hpp"

namespace KNW {

	class IOTCP : public boost::enable_shared_from_this<KNW::IOTCP> {

	public:
		IOTCP() = delete;

		using b_sptr = boost::shared_ptr<IOTCP>;
		using b_wptr = boost::weak_ptr<IOTCP>;

		static boost::shared_ptr<KNW::IOTCP> create(
				DataTCP::b_wptr dataTCP_,
				boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
				std::function<void()> callbackDeadSocket
				);

		void writeSocket(std::string data);
		void writeSocket(const void *pVoid, size_t len);

		void readSocketHeader();

		const boost::shared_ptr<boost::asio::ip::tcp::socket> &getSocket_();

		virtual ~IOTCP();

		bool isConnect() const;
	private:
		IOTCP(
				DataTCP::b_wptr dataTCP_,
				boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
				std::function<void()> callbackDeadSocket
		);

		void checkError(boost::system::error_code const &error_code);

		void readSocketData(DataTCP::Header header);

		void handleReadHeader(const boost::system::error_code &ec, size_t len);


		void handleReadData(DataTCP::Header header, const boost::system::error_code &, size_t len);

		void handleWrite(const boost::system::error_code &, size_t len);

		DataTCP::b_wptr dataTCP_;
		boost::array<char, eConfigTCP::kMaxBufferSize> buffer_data_;
		boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
		std::function<void()> callbackDeadSocket_;
	};
}


