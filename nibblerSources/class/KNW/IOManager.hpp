#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>


class IOManager {
public:
	IOManager(size_t thread_size = 1);
	IOManager() = delete;
	~IOManager();
	IOManager &operator=(const IOManager &) = delete;
	IOManager(const IOManager &) = delete;

	boost::asio::io_service &getIo();

	boost::thread_group &getThreadGroup();


private:

	void IORunner();
	boost::mutex global_stream_lock;
	boost::shared_ptr<boost::asio::io_service> io;
	boost::thread_group thread_group;
	boost::asio::io_service::work work_;
};


