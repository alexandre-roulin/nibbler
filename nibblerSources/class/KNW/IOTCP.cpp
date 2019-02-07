#include <utility>

#include "IOTCP.hpp"
#include "ServerTCP.hpp"
#include "DataTCP.hpp"


KNW::IOTCP::IOTCP(
		KNW::DataTCP &dataTCP,
		tcp::socket socket,
		std::function<void(BaseDataType::Header, char *)> f,
		std::function<void()> callbackDeadSocket)
		:
		openObject_(true),
		dataTCP_(dataTCP) ,
		socket_(std::move(socket)),
		callback_(std::move(f)),
		callbackDeadSocket_(callbackDeadSocket) {

}

void KNW::IOTCP::readSocketHeader() {
//	log_fatal("%s", __PRETTY_FUNCTION__);
	boost::asio::async_read(
			socket_,
			boost::asio::buffer(buffer_data_, sizeof(BaseDataType::Header)),
			boost::bind(
					&KNW::IOTCP::handleReadHeader,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
			));

}

void KNW::IOTCP::handleReadHeader(const boost::system::error_code &ec,
									 size_t len) {
//	log_fatal("%s", __PRETTY_FUNCTION__);
	if (ec.value() == 0) {
		BaseDataType::Header header;
		std::memcpy(&header, buffer_data_.data(), len);
		readSocketData(header);
	} else {
		checkError(ec);
		std::cout << "handleReadHeader::error" << ec.message() << std::endl;
	}
}


void KNW::IOTCP::readSocketData(BaseDataType::Header header) {
//	log_fatal("%s", __PRETTY_FUNCTION__);
	boost::asio::async_read(
			socket_,
			boost::asio::buffer(buffer_data_, dataTCP_.getSizeOfHeader(header)),
			boost::bind(
					&KNW::IOTCP::handleReadData,
					shared_from_this(),
					header,
					boost::asio::placeholders::error
			));
}


void KNW::IOTCP::handleReadData(BaseDataType::Header header,
								   const boost::system::error_code &ec) {
//	log_fatal("%s", __PRETTY_FUNCTION__);
	if (ec.value() == 0) {
		callback_(header, buffer_data_.data());
		readSocketHeader();
	} else {
		checkError(ec);
		std::cout << "handleReadData::error" << std::endl;
	}
}

void
KNW::IOTCP::handleWrite(const boost::system::error_code &ec) {
//	log_fatal("%s", __PRETTY_FUNCTION__);
	if (ec.value() != 0) {
		checkError(ec);
		std::cout << __PRETTY_FUNCTION__ << ec.message() << std::endl;
	}
}

void KNW::IOTCP::writeSocket(std::string data) {
	boost::asio::async_write(socket_, boost::asio::buffer(data),
							 boost::bind(&KNW::IOTCP::handleWrite,
										 shared_from_this(),
										 boost::asio::placeholders::error));
}

void KNW::IOTCP::checkError(boost::system::error_code const &error_code) {
//	if (!openObject_)
//		return;
	log_fatal("%s addr %s %d", __PRETTY_FUNCTION__, error_code.message().c_str(), socket_.is_open());
	try {
		if (error_code.value() != 0) {
			if (socket_.is_open())
				socket_.close();
			if (callbackDeadSocket_) {
				callbackDeadSocket_();
			}
		}
	} catch (std::exception const &e) {
		std::cout << e.what() << std::endl;
	}
}

const tcp::socket &KNW::IOTCP::getSocket_() const {
	return socket_;
}

KNW::IOTCP::~IOTCP() {
	openObject_ = false;
	log_warn("%s %d", __PRETTY_FUNCTION__, socket_.is_open());
	try {
		if (socket_.is_open()) {
			socket_.close();
		}
	} catch (std::exception const &e) {
		std::cout << e.what() << std::endl;
	}
	if (callbackDeadSocket_)
		callbackDeadSocket_();
}

bool KNW::IOTCP::isConnect() const {
	return socket_.is_open();
}

boost::shared_ptr<KNW::IOTCP>
KNW::IOTCP::create(KNW::DataTCP &dataTCP_, tcp::socket socket_,
				   std::function<void(BaseDataType::Header, char *)> f,
				   std::function<void()> callbackDeadSocket) {

	return boost::shared_ptr<KNW::IOTCP>(new IOTCP(dataTCP_, std::move(socket_), f, callbackDeadSocket));
}
