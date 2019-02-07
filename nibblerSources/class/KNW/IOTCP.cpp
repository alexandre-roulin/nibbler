#include <utility>

#include "IOTCP.hpp"
#include "ServerTCP.hpp"


KNW::IOTCP::IOTCP(
		DataTCP::boost_weak_ptr dataTCP,
		tcp::socket socket,
		std::function<void()> callbackDeadSocket)
		:
		dataTCP_(dataTCP),
		socket_(std::move(socket)),
		callbackDeadSocket_(callbackDeadSocket) {

}

void KNW::IOTCP::readSocketHeader() {
	log_fatal("%s isOpen %d ptr %d", __PRETTY_FUNCTION__, socket_.is_open());
	boost::weak_ptr<KNW::IOTCP> w(weakPtr);

	log_fatal("%s isOpen %d ptr %d", __PRETTY_FUNCTION__, socket_.is_open(), weakPtr.lock() !=
			nullptr);

	boost::asio::async_read(
			socket_,
			boost::asio::buffer(buffer_data_, sizeof(BaseDataType::Header)),
			[&, w](boost::system::error_code ec, size_t len) {
				auto ptr = w.lock();
				log_info("hptr %d", ptr != nullptr);
				if (ptr)
					ptr->handleReadHeader(ec, len);
			});

}

void
KNW::IOTCP::handleReadHeader(const boost::system::error_code &ec, size_t len) {
	log_fatal("%s", __PRETTY_FUNCTION__);
	if (ec.value() == 0 && len != 0) {
		BaseDataType::Header header;
		std::memcpy(&header, buffer_data_.data(), len);
		readSocketData(header);
	} else {
		checkError(ec);
		std::cout << "handleReadHeader::error" << ec.message() << std::endl;
	}
}


void KNW::IOTCP::readSocketData(BaseDataType::Header header) {
	log_fatal("%s", __PRETTY_FUNCTION__);

	DataTCP::boost_shared_ptr sharedPtr = dataTCP_.lock();
	boost_weak_ptr w = weakPtr;
	if (sharedPtr)
		boost::asio::async_read(
				socket_,
				boost::asio::buffer(buffer_data_, sharedPtr->getSizeOfHeader(header)),
				[&, w, header](boost::system::error_code ec, size_t len) {
					auto ptr = w.lock();
					log_info("dptr %d", ptr != nullptr);
					if (ptr) ptr->handleReadData(header, ec, len);
				});
}


void KNW::IOTCP::handleReadData(
		BaseDataType::Header header,
		const boost::system::error_code &ec,
		size_t) {
	log_fatal("%s", __PRETTY_FUNCTION__);

	if (ec.value() == 0) {
		auto ptr = dataTCP_.lock();
		if (ptr)
			ptr->sendDataToCallback(header, buffer_data_.data());
		readSocketHeader();
	} else {
		checkError(ec);
		std::cout << "handleReadData::error" << std::endl;
	}
}

void
KNW::IOTCP::handleWrite(const boost::system::error_code &ec, size_t len) {
	log_fatal("%s ec %d len %d", __PRETTY_FUNCTION__, ec.value(), len);
	if (ec.value() != 0 || len == 0) {
		checkError(ec);
	}
}

void KNW::IOTCP::writeSocket(std::string data) {
	log_fatal("%s", __PRETTY_FUNCTION__);

	boost_weak_ptr w = weakPtr;
	boost::asio::async_write(
			socket_,
			boost::asio::buffer(data),
			[&, w](boost::system::error_code ec, size_t len) {
				auto ptr = w.lock();
				log_info("wptr %d", ptr != nullptr);
				if (ptr) ptr->handleWrite(ec, len);
			});
}

void KNW::IOTCP::checkError(boost::system::error_code const &error_code) {

	log_fatal("%s %s %d cb %d", __PRETTY_FUNCTION__,
			  error_code.message().c_str(), socket_.is_open(),
			  callbackDeadSocket_ != nullptr);
	if (error_code.value() != 0) {
		try {
			if (socket_.is_open())
				socket_.close();
		} catch (std::exception const &e) {
			std::cout << e.what() << std::endl;
		}
		if (callbackDeadSocket_) {
			callbackDeadSocket_();
			callbackDeadSocket_ = nullptr;
		}
	}
}

const tcp::socket &KNW::IOTCP::getSocket_() const {
	return socket_;
}

KNW::IOTCP::~IOTCP() {
	log_warn("%s", __PRETTY_FUNCTION__);
	try {
		if (socket_.is_open()) {
			socket_.close();
		}
	} catch (std::exception const &e) {
		std::cout << e.what() << std::endl;
	}
	log_success("callbackDeadSocket_ %d", callbackDeadSocket_ != nullptr);
	if (callbackDeadSocket_)
		callbackDeadSocket_();
	log_success("callbackDeadSocket_");

}

bool KNW::IOTCP::isConnect() const {
	return socket_.is_open();
}

boost::shared_ptr<KNW::IOTCP>
KNW::IOTCP::create(KNW::DataTCP::boost_weak_ptr dataTCP_, tcp::socket socket_,
				   std::function<void()> callbackDeadSocket) {
	auto ptr = boost::shared_ptr<KNW::IOTCP>(new IOTCP(dataTCP_, std::move(socket_), std::move(callbackDeadSocket)));
	ptr->weakPtr = ptr;
	return ptr;
}
