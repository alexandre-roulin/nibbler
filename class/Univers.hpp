#pragma once

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <bitset>
#include "IDisplay.hpp"
#include "ISound.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <events/NextFrame.hpp>

class ServerTCP;

class ClientTCP;

class Core;
namespace KINU {

	class World;
};

class Univers {
public:
	
	enum eFlag { SOUND };
	
	Univers();

	bool load_external_display_library(std::string const &title, std::string const &library_path);
	bool load_external_sound_library(std::string const &title, std::string const &library_path);

	void loop();

	void manage_start();

	void manage_input();

	unsigned int &getMapSize();

	KINU::World &getWorld_() const;

	ClientTCP &getClientTCP_() const;
	
	ServerTCP &getServerTCP_() const;

	Core &getCore_() const;
	Core *releaseCore_();
	
	ISound &getSound() const;
	void	playNoise(int i) const;
	void	playNoise(eSound e) const;
	void	playMusic(char *path) const;
	void	playMusic(std::string const &path) const;

	void create_ui();
	bool isServer() const;

	void create_server(unsigned int port = 4242);

	bool dlError(void);
	void setFlag(eFlag);
	void unsetFlag(eFlag);
	bool testFlag(eFlag);

private:

	void loop_world();

	std::vector<NextFrame> nextFrame;
	std::bitset<32>			flag;
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
	void *dlHandleDisplay;
	void *dlHandleSound;
	IDisplay	*display;
	ISound		*sound;
	bool		isServer_;
	std::mutex mutex;
	bool borderless;
public:
	bool isBorderless() const;

	void setBorderless(bool borderless);

private:
	unsigned int mapSize;
public:
	void setMapSize(unsigned int mapSize);

private:
	unsigned int gameSpeed;
	time_t start = time(NULL);
private:

	IDisplay *(*newDisplay)(char const *, int, int, int, char const *);
	ISound *(*newSound)(char const *);

	void (*deleteDisplay)(IDisplay *);
	void (*deleteSound)(ISound *);

};


