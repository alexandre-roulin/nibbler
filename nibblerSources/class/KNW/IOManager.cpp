#include "IOManager.hpp"

IOManager::IOManager()
		: io(new boost::asio::io_service),
		  work(new boost::asio::io_service::work(*io)) {
	for (int index = 0; index < 4; ++index) {
		thread_group.create_thread( boost::bind( &IOManager::IORunner, this) );
	}
}

void IOManager::IORunner() {
	io->run();
}

boost::asio::io_service &IOManager::getIo() {
	return *io;
}

boost::thread_group &IOManager::getThreadGroup() {
	return thread_group;
}

IOManager::~IOManager() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	io->stop();
	thread_group.join_all();
}
