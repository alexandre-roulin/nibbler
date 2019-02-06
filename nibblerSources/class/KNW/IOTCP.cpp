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
					this,
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
					this,
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
										 this,
										 boost::asio::placeholders::error));
}

void KNW::IOTCP::checkError(boost::system::error_code const &error_code) {
//	log_fatal("%s %d", __PRETTY_FUNCTION__, error_code.value());
	try {
		if (error_code.value() != 0) {
			socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
			socket_.close();
			if (callbackDeadSocket_)
				callbackDeadSocket_();
		}
	} catch (std::exception const &e) {
		if (callbackDeadSocket_)
			callbackDeadSocket_();
		std::cout << e.what() << std::endl;
	}
}

const tcp::socket &KNW::IOTCP::getSocket_() const {
	return socket_;
}

KNW::IOTCP::~IOTCP() {
	log_warn("%s %d", __PRETTY_FUNCTION__, socket_.is_open());
	try {
		if (socket_.is_open()) {
			socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
			socket_.close();
		}
		if (callbackDeadSocket_)
			callbackDeadSocket_();
	} catch (std::exception const &e) {
		if (callbackDeadSocket_)
			callbackDeadSocket_();
		std::cout << e.what() << std::endl;
	}
}

bool KNW::IOTCP::isConnect() const {
	return socket_.is_open();
}
