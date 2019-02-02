#include <utility>

#include "IOTCP.hpp"
#include "ServerTCP.hpp"
#include "DataTCP.hpp"


KNW::IOTCP::IOTCP(
		KNW::DataTCP &dataTCP,
		tcp::socket socket,
		std::function<void(BaseDataType::Header, char *)> f)
		:
		callback_(std::move(f)),
		socket_(std::move(socket)),
		dataTCP_(dataTCP) {


}

void KNW::IOTCP::readSocketHeader() {
	log_fatal("%s", __PRETTY_FUNCTION__);
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

	log_fatal("%s", __PRETTY_FUNCTION__);
	checkError(ec);
	if (ec.value() == 0) {
		BaseDataType::Header header;
		std::memcpy(&header, buffer_data_.data(), len);
		readSocketData(header);
	} else {
		std::cout << "handleReadHeader::error" << ec.message() << std::endl;
	}
}


void KNW::IOTCP::readSocketData(BaseDataType::Header header) {
	log_fatal("%s", __PRETTY_FUNCTION__);
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
	log_fatal("%s", __PRETTY_FUNCTION__);
	checkError(ec);
	if (ec.value() == 0) {
		callback_(header, buffer_data_.data());
		readSocketHeader();
	} else {
		std::cout << "handleReadData::error" << std::endl;
	}
}

void
KNW::IOTCP::handleWrite(const boost::system::error_code &ec, size_t len) {
	log_fatal("%s", __PRETTY_FUNCTION__);
	checkError(ec);
	if (ec.value() != 0)
		std::cout << "error" << std::endl;
}

void KNW::IOTCP::writeSocket(std::string data) {
	boost::asio::async_write(socket_, boost::asio::buffer(data),
							 boost::bind(&KNW::IOTCP::handleWrite,
										 this,
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void KNW::IOTCP::checkError(boost::system::error_code const &error_code) {
	log_fatal("%s %d", __PRETTY_FUNCTION__, error_code.value());
	try {
		if ((boost::asio::error::eof == error_code) ||
			(boost::asio::error::connection_reset == error_code)) {
			socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
			socket_.close();
		}
	} catch (std::exception const &e) {
		std::cout << e.what() << std::endl;
	}
}
