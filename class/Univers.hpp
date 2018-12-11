#pragma once

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <bitset>
#include "IDisplay.hpp"
#include "ISound.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <events/NextFrame.hpp>
#include <boost/thread.hpp>
class ServerTCP;

class ClientTCP;

class Core;

namespace KINU {

	class World;
};

class Univers {
public:

	enum eDisplay {
		EXTERN_LIB_SFML,
		EXTERN_LIB_SDL,
		EXTERN_LIB_STB
	};

	enum eFlag {
		SOUND
	};

	Univers();

	bool load_extern_lib_display(eDisplay);

	bool load_external_sound_library(std::string const &title,
									 std::string const &library_path); // TODO GO PRIVATE
	void unload_external_library();
	void loop();

	void manage_start();

	void new_game();

	Core &getCore_() const;

	Core *releaseCore_();

	/** Sound **/

	void playNoise(int i) const;

	void playNoise(eSound e) const;

	void playMusic(char *path) const;

	void playMusic(std::string const &path) const;

	/** Create function**/

	void create_server(unsigned int port = 4242);

	void create_ui(); // TODO PRIVATE


	/** Setter && Getter**/

	//Network

	ClientTCP &getClientTCP_() const;

	//Game

	KINU::World &getWorld_() const;

	void setMapSize(unsigned int mapSize);

	bool isBorderless() const;

	void setBorderless(bool borderless);

	unsigned int getMapSize() const;

	//Flag

	void setFlag(eFlag);

	void unsetFlag(eFlag);

	bool testFlag(eFlag);

	//Sound

	ISound &getSound() const;

	//State

	bool isServer() const;


private: // Function
	void manage_input();

	void loop_world();

	bool dlError(void);

private: // Variable

	std::vector<NextFrame> nextFrame;
	std::bitset<32> flag;
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
	IDisplay *display;
	ISound *sound;
	bool borderless;
	boost::thread thread;

	unsigned int mapSize;
	unsigned int gameSpeed;

	bool load_external_display_library(std::string const &title,
									   std::string const &libPath);

	IDisplay *(*newDisplay)(char const *, int, int, int, char const *);

	ISound *(*newSound)(char const *);

	void (*deleteDisplay)(IDisplay *);

	void (*deleteSound)(ISound *);

};


