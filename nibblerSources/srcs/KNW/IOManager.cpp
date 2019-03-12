#include "IOManager.hpp"

IOManager::IOManager()
		:
		io(new boost::asio::io_service),
		work_(*io){

	for (size_t index = 0; index < 10; ++index) {
		thread_group.create_thread( boost::bind( &IOManager::IORunner, this) );
	}
}

void IOManager::IORunner() {
	io->run();
}

boost::asio::io_service &IOManager::getIo() {
	IOManager *pVoid = this;
	if (pVoid == nullptr) {
		std::cerr << "nibbler: error: IO Overflow" << std::endl;
		exit(1);
	}
	return *io;
}

boost::thread_group &IOManager::getThreadGroup() {
	return thread_group;
}

IOManager::~IOManager() {
	io->stop();
	thread_group.join_all();
}

void IOManager::startIORunner() {

}
