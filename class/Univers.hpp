#pragma once

#include <KNU/World.hpp>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>

class ServerTCP;
class ClientTCP;
class Core;

class Univers {

public:
	Univers();

	KNU::World &getWorld_() const;

	ClientTCP &getClientTCP_() const;

	ServerTCP &getServerTCP_() const;

	Core &getCore_() const;
	void create_ui();

	const Snake *getSnakes() const;

	void create_server();
	void create_client();


private:
	ServerTCP *serverTCP;


	boost::asio::io_service io_server;
	boost::asio::io_service io_client;

	std::unique_ptr<KNU::World> world_;
	boost::shared_ptr<ClientTCP> clientTCP_;
	std::unique_ptr<ServerTCP> serverTCP_;
	std::unique_ptr<Core> core_;
};


