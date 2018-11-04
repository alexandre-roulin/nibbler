#include "Univers.hpp"
#include <gui/Core.hpp>
#include <network/ClientTCP.hpp>

Univers::Univers() {
	world_ = std::make_unique<KNU::World>(*this);
	core_ = nullptr;
	clientTCP_ = nullptr;
	serverTCP_ = nullptr;
}

KNU::World &Univers::getWorld_() const {
	return *world_;
}

ClientTCP &Univers::getClientTCP_() const {
	return *clientTCP_;
}

ServerTCP &Univers::getServerTCP_() const {
	return *serverTCP_;
}

Core &Univers::getCore_() const {
	return *core_;
}

void Univers::create_ui() {
	core_ = std::make_unique<Core>(*this);
}
void Univers::create_server() {
	serverTCP_ = std::make_unique<ServerTCP>(*this, io_server);
	boost::thread t2(boost::bind(&boost::asio::io_service::run, &io_server));
	t2.detach();
}

void Univers::create_client() {
	std::string buffer;
	std::cout << "Connect : ";
	std::getline(std::cin, buffer);
	clientTCP_ = ClientTCP::create(*this, io_client, std::string(buffer.c_str()));
	clientTCP_->connect();
	clientTCP_->read_socket_header();
	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_client));
	t.detach();
}
