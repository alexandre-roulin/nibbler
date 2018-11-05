#pragma once

#include <KNU/World.hpp>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include "../externlib/commun/IDisplay.hpp"
#include <boost/asio/deadline_timer.hpp>

class ServerTCP;

class ClientTCP;

class Core;

class Univers {
public:

	Univers();

	IDisplay &getDisplay() const;

	int start_game();

	void loop();


	void manage_input();


	KNU::World &getWorld_() const;

	ClientTCP &getClientTCP_() const;

	ServerTCP &getServerTCP_() const;

	Core &getCore_() const;

	void create_ui();

	const Snake *getSnakes() const;

	void create_server();

	void create_client();

	bool dlError(void);

private:

	void loop_world();
	ServerTCP *serverTCP;
	boost::asio::io_service io_server;
	boost::asio::io_service io_client;

	boost::asio::io_service io;
	boost::asio::deadline_timer deadline_timer = boost::asio::deadline_timer(io,
																			 boost::posix_time::seconds(
																					 0));
	std::unique_ptr<KNU::World> world_;
	boost::shared_ptr<ClientTCP> clientTCP_;
	std::unique_ptr<ServerTCP> serverTCP_;
	std::unique_ptr<Core> core_;
	void *dlHandle;
	IDisplay *display;

	IDisplay *(*newDisplay)(char const *, int, int, int, char const *);

	void (*deleteDisplay)(IDisplay *);

};


