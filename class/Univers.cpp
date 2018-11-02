#include "Univers.hpp"
#include <gui/Core.hpp>
#include <network/ClientTCP.hpp>

Univers::Univers() {
	world_ = std::make_unique<KNU::World>(*this);
	core_ = std::make_unique<Core>(*this);
	clientTCP_ = nullptr;
	serverTCP_ = nullptr;
	core_->aState();
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

void Univers::create_server() {
	serverTCP_ = std::make_unique<ServerTCP>(*this, io_server);
	boost::thread t2(boost::bind(&boost::asio::io_service::run, &io_server));
	t2.detach();
}

void Univers::create_client() {
	clientTCP_ = ClientTCP::create(*this, io_client, std::string("localhost"));

	clientTCP_->connect();
	clientTCP_->read_socket();
	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_client));
	t.detach();
}
