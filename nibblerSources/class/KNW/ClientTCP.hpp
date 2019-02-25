#pragma once


#include <boost/asio.hpp>
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "IOManager.hpp"
#include "IOTCP.hpp"
#include "Config.hpp"

namespace KNW {

	class ClientTCP : public boost::enable_shared_from_this<ClientTCP> {
	public:
		using b_sptr = boost::shared_ptr<ClientTCP>;
		using b_wptr = boost::weak_ptr<ClientTCP>;

		void connect(const std::string host, const std::string port);

		static b_sptr
		create(IOManager &io_manager, std::function<void()> = nullptr);

		bool isConnect() const;

		template<typename T>
		void writeDataToServer(T &&data);

		template<typename T, typename H>
		void writeDataToServer(T &&data, H header);

		void disconnect();
		virtual ~ClientTCP();

		DataTCP &getDataTCP_();

	private:

		explicit ClientTCP(IOManager &);

		DataTCP::b_sptr dataTCP_;
		IOManager &io_manager_;
		boost::shared_ptr<IOTCP> iotcp;
		std::function<void()> callbackDeadConnection_;
	};

	template<typename T, typename H>
	void ClientTCP::writeDataToServer(T &&data, H header) {
		assert(iotcp != nullptr);
		iotcp->writeSocket(dataTCP_->serializeData(header, data));
	}

	template<typename T>
	void ClientTCP::writeDataToServer(T &&data) {
		assert(dataTCP_->hasType<T>());
		iotcp->writeSocket(dataTCP_->serializeData(DataType<T>::getHeader(), data));
	}

}