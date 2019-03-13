#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>


class IOManager {
public:
	using b_sptr = boost::shared_ptr<IOManager>;
	using b_wptr = boost::weak_ptr<IOManager>;

	static b_sptr create();
	IOManager();
	~IOManager();
	IOManager &operator=(const IOManager &) = delete;
	IOManager(const IOManager &) = delete;

	boost::asio::io_service &getIo();
	boost::thread_group &getThreadGroup();

private:

	void IORunner();
	boost::shared_ptr<boost::asio::io_service> io;
	boost::thread_group thread_group;
	boost::asio::io_service::work work_;
};


