#include <utility>
#include "IOTCP.hpp"
#include "ServerTCP.hpp"
#include <logger.h>

namespace KNW {

	IOTCP::IOTCP(
			DataTCP::b_wptr dataTCP,
			boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
			std::function<void()> callbackDeadSocket)
			:
			dataTCP_(std::move(dataTCP)),
			socket_(std::move(socket)),
			callbackDeadSocket_(std::move(callbackDeadSocket)) {
	}

	void IOTCP::readSocketHeader() {
		boost::weak_ptr<IOTCP> weakPtr(shared_from_this());
		boost::asio::async_read(
				*socket_,
				boost::asio::buffer(buffer_data_, sizeof(BaseDataType::Header)),
				[weakPtr](boost::system::error_code ec, size_t len) {
					auto ptr = weakPtr.lock();
					if (ptr) ptr->handleReadHeader(ec, len);
				});

	}

	void
	IOTCP::handleReadHeader(
			const boost::system::error_code &ec,
			size_t len) {

		if (ec.value() == 0 && len != 0) {
			BaseDataType::Header header;
			std::memcpy(&header, buffer_data_.data(), len);
			readSocketData(header);
		} else {
			checkError(ec);
		}
	}


	void IOTCP::readSocketData(BaseDataType::Header header) {

		DataTCP::b_sptr sharedPtr = dataTCP_.lock();
		if (sharedPtr) {
			boost::weak_ptr<IOTCP> weakPtr(weak_from_this());

			boost::asio::async_read(
					*socket_,
					boost::asio::buffer(buffer_data_,
										static_cast<size_t>(sharedPtr->getSizeOfHeader(header))),
					[weakPtr, header](boost::system::error_code ec, size_t len) {
						auto ptr = weakPtr.lock();
						if (ptr) ptr->handleReadData(header, ec, len);
					});
		}
	}


	void IOTCP::handleReadData(
			BaseDataType::Header header,
			const boost::system::error_code &ec,
			size_t) {

		if (ec.value() == 0) {
			auto ptr = dataTCP_.lock();
			if (ptr)
				ptr->sendDataToCallback(header, buffer_data_.data());
			readSocketHeader();
		} else {
			checkError(ec);
		}
	}

	void
	IOTCP::handleWrite(const boost::system::error_code &ec, size_t) {
		checkError(ec);
	}

	void IOTCP::writeSocket(std::string data) {
//	log_fatal("%s", __PRETTY_FUNCTION__);

		boost::weak_ptr<IOTCP> weakPtr(shared_from_this());
		boost::asio::async_write(
				*socket_,
				boost::asio::buffer(data),
				[weakPtr](boost::system::error_code ec, size_t len) {
					auto ptr = weakPtr.lock();
					if (ptr) ptr->handleWrite(ec, len);
				});
	}

	void IOTCP::checkError(boost::system::error_code const &error_code) {
		log_info("%s %d", __PRETTY_FUNCTION__, error_code.value());
		if (error_code) {
			if (callbackDeadSocket_)
				callbackDeadSocket_();
		}
	}


	bool IOTCP::isConnect() const {
		return socket_->is_open();
	}
	IOTCP::b_sptr IOTCP::create(
			DataTCP::b_wptr dataTCP_,
			boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
			std::function<void()> callbackDeadSocket) {
		auto ptr = boost::shared_ptr<IOTCP>(new IOTCP(
				std::move(dataTCP_),
				std::move(socket),
				std::move(callbackDeadSocket)));

		ptr->readSocketHeader();
		return ptr;
	}

	boost::asio::ip::tcp::socket &IOTCP::getSocket_() {
		return *socket_;
	}

	IOTCP::~IOTCP() = default;
}