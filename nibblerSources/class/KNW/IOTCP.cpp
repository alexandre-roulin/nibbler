#include <utility>
#include "IOTCP.hpp"
#include "ServerTCP.hpp"

namespace KNW {

	IOTCP::IOTCP(
			DataTCP::b_wptr dataTCP,
			boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
			std::function<void()> callbackDeadSocket)
			:
			dataTCP_(std::move(dataTCP)),
			socket_(std::move(socket)),
			callbackDeadSocket_(std::move(callbackDeadSocket)),
			open_(true){
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

		if (!open_) return;

		BaseDataType::Header header;
		std::memcpy(&header, buffer_data_.data(), len);
		if (ec.value() != 0 || header == 0) {
			checkError(ec);
		} else {
			readSocketData(header);
		}
	}


	void IOTCP::readSocketData(BaseDataType::Header header) {

		if (!open_) return;
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
		if (ec.value() != 0)
			checkError(ec);
	}

	void IOTCP::writeSyncSocket(std::string data) {
		boost::system::error_code ec;
		boost::asio::write(*socket_, boost::asio::buffer(data), ec);
	}

	void IOTCP::writeSocket(std::string data) {
		if (!open_) return;

		boost::weak_ptr<IOTCP> weakPtr(shared_from_this());
		if (socket_ && socket_->is_open())
		boost::asio::async_write(
				*socket_,
				boost::asio::buffer(data),
				[weakPtr](boost::system::error_code ec, size_t len) {
					auto ptr = weakPtr.lock();
					if (ptr) ptr->handleWrite(ec, len);
				});
	}

	void IOTCP::writeSocket(const void *pVoid, size_t len) {
		boost::weak_ptr<IOTCP> weakPtr(shared_from_this());
		if (!open_) return;
		boost::asio::async_write(
				*socket_,
				boost::asio::buffer(pVoid, len),
				[weakPtr](boost::system::error_code ec, size_t len) {
					auto ptr = weakPtr.lock();
					if (ptr) ptr->handleWrite(ec, len);
				});
	}

	void IOTCP::checkError(boost::system::error_code const &) {
		open_ = false;
		if (callbackDeadSocket_)
			callbackDeadSocket_();
	}


	bool IOTCP::isConnect() const {
		return socket_->is_open() && open_;
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

	const boost::shared_ptr<boost::asio::ip::tcp::socket> &IOTCP::getSocket_() {
		return socket_;
	}


	IOTCP::~IOTCP() = default;
}