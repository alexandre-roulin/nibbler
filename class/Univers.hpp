#pragma once

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include "IDisplay.hpp"
#include <boost/asio/deadline_timer.hpp>


class ServerTCP;

class ClientTCP;

class Core;
namespace Mix {

	class World;
};

class Univers {
public:
	Univers();

	IDisplay &getDisplay() const;

	int start_game();

	void loop();


	void manage_input();


	Mix::World &getWorld_() const;

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
																					 1));
	std::unique_ptr<Mix::World> world_;
	boost::shared_ptr<ClientTCP> clientTCP_;
	std::unique_ptr<ServerTCP> serverTCP_;
	std::unique_ptr<Core> core_;
	void *dlHandle;
	IDisplay *display;

	IDisplay *(*newDisplay)(char const *, int, int, int, char const *);

	void (*deleteDisplay)(IDisplay *);

};


