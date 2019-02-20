//
// Created by Noe TONIOLO on 2019-02-18.
//

#ifndef PROJECT_TEST_HPP
#define PROJECT_TEST_HPP

#include <fstream>
#include <functional>

#define INPUT_ID 0
#define INPUT_ACTION 1
#define INPUT_WAITING_TIME 500000

class Test {
public:
	static Test &getInstance();


	void setPidTestProcess(int pidTestProcess);
	void setId(int id);
	void setInputFile(std::string const &inputFile);

	void setInput(bool b);
	void setInputCallback(std::function<void(void *, std::string const &)> const &callback, void *ptr);
	void setTest(bool b);

	void writeInput(std::string const &s);
	bool needUpdate() const;

	void update();

private:
	static Test m_instance;

	Test();
	~Test();
	static void sigHandler_(int signo);

	bool test_;
	bool input_;
	int id_;
	int pidTestProcess_;
	bool next_;
	std::ifstream iAction_;
	std::ofstream oAction_;
	std::string buffer_;
	std::function<void(void *, std::string const &)> callback_;
	void *ptr_;
};

#endif //PROJECT_TEST_HPP
