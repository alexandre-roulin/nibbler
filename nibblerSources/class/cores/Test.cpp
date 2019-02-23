//
// Created by Noe TONIOLO on 2019-02-18.
//

#include "Test.hpp"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cassert>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>


using namespace std::chrono_literals;


Test &Test::getInstance() {
	return (Test::m_instance);
}

void Test::sigHandler_(int signo) {
	if (signo == SIGUSR1) {
		std::cout << ": received SIGUSR1" << std::endl;
		Test::getInstance().next_ = true;
	} else
		exit(signo);
}

void Test::writeInput(std::string const &s) {
	if (input_)
		oAction_ << id_ << " " << s << std::endl;
}

bool Test::needUpdate() const {
	return next_;
}

void Test::update() {
	if (!test_ || !next_) {
		return ;
	}
	next_ = false;
	std::vector<std::string> token;

	boost::split(token, buffer_, boost::is_any_of(" "));

	assert(token.size() >= 2);

	if (stoi(token[INPUT_ID]) == id_) {
		std::cout << id_ << ": Is my action [" << token[INPUT_ACTION] << "]" << std::endl;
		callback_(ptr_, token[INPUT_ACTION]);
		std::cout << "Continue" << std::endl;
		//usleep(INPUT_WAITING_TIME);
		std::this_thread::sleep_for(300ms);
		kill(pidTestProcess_, SIGUSR1);
	} else {
		std::cout << id_;
		for (auto tok : token) {
			std::cout << "[" << tok << "]";
		}
		std::cout << std::endl;
	}
	if (!std::getline(iAction_, buffer_)) {
		std::cout << id_ << "[END]" << std::endl;
		std::cout << id_ << "[sleep]" << std::endl;
		std::cout.flush();
		std::this_thread::sleep_for(500ms * id_);
		kill(pidTestProcess_, SIGUSR2);
		//kill(pidTestProcess_, SIGUSR1);
		std::cout << id_ << "[end sleep]" << std::endl;
		std::this_thread::sleep_for(5s);
		if (1 && printf("l"))
			exit(EXIT_SUCCESS);
	}
}

void Test::addLog(std::string const &s) {
	oLogs_ << id_ << " " << s << std::endl;
}

void Test::setTest(bool b) {
	test_ = b;
}
void Test::setInput(bool b) {
	input_ = b;
}
void Test::setInputCallback(std::function<void(void *, std::string const &)> const &callback, void *ptr) {
	ptr_ = ptr;
	callback_ = callback;
}
void Test::setId(int id) {
	id_ = id;
}
void Test::setPidTestProcess(int pidTestProcess) {
	pidTestProcess_ = pidTestProcess;
}
void Test::setInputFile(std::string const &inputFile) {
	if (test_) {
		iAction_.open((path_ / TEST_DIR / inputFile / (inputFile + ".ut")).generic_string());
		std::getline(iAction_, buffer_);
	}
	if (input_){
		if (!boost::filesystem::exists(path_ / TEST_DIR / inputFile))
			boost::filesystem::create_directory(path_ / TEST_DIR / inputFile);
		oAction_.open((path_ / TEST_DIR / inputFile / (inputFile + ".ut")).generic_string(), std::ios_base::app);
	}
}

void Test::setLogFile(std::string const &logFile) {
	if (input_) {
		if (!boost::filesystem::exists(path_ / TEST_DIR / logFile))
			boost::filesystem::create_directory(path_ / TEST_DIR / logFile);
		oLogs_.open((path_ / TEST_DIR / logFile / (logFile + std::to_string(id_) + ".log")).generic_string(), std::ios_base::app);
	}
	else
		oLogs_.open((path_ / TEST_OUTPUT / (logFile + std::to_string(id_) + ".log")).generic_string(), std::ios_base::app);
}


Test::Test() :
path_(boost::filesystem::path(NIBBLER_ROOT_PROJECT_PATH)),
test_(0),
input_(0),
id_(0),
pidTestProcess_(0),
next_(false),
ptr_(nullptr) {
	if (signal(SIGUSR1, Test::sigHandler_) == SIG_ERR)
		printf("\ncan't catch SIGUSR1\n");
}

Test Test::m_instance=Test();