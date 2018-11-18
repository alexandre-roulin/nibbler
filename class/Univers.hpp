#pragma once

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include "IDisplay.hpp"
#include <boost/asio/deadline_timer.hpp>


class ServerTCP;

class ClientTCP;

class Core;
namespace KINU {

	class World;
};

class Univers {
public:
	Univers();

	bool load_external_library(std::string title, std::string library_path);

	void loop();

	void manage_start();

	void manage_input();

	unsigned int getMapSize();

	KINU::World &getWorld_() const;

	ClientTCP &getClientTCP_() const;

	Core &getCore_() const;

	void create_ui();

	void create_server(unsigned int port = 4242);

	bool dlError(void);

private:

	void loop_world();

	boost::asio::io_service io_server;
	boost::asio::io_service io_client;
	boost::asio::io_service io_loop;
	boost::asio::io_service io_start;
	boost::asio::deadline_timer timer_loop;
	boost::asio::deadline_timer timer_start;
	std::unique_ptr<KINU::World> world_;
	std::unique_ptr<ServerTCP> serverTCP_;
	std::unique_ptr<Core> core_;
	boost::shared_ptr<ClientTCP> clientTCP_;
	void *dlHandle;
	IDisplay *display;

	unsigned int mapSize;
public:
	void setMapSize(unsigned int mapSize);

private:
	unsigned int gameSpeed;

private:

	IDisplay *(*newDisplay)(char const *, int, int, int, char const *);

	void (*deleteDisplay)(IDisplay *);

};


