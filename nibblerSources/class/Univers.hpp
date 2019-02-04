#pragma once

#include <ia/Bobby.hpp>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/filesystem.hpp>
#include <bitset>
#include "nibbler.hpp"
#include "ISound.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <events/NextFrame.hpp>
#include <boost/thread.hpp>

#define PATH_DISPLAY_LIBRARY_SFML "dynamicLibraries/libdisplay_sfml.so"
#define PATH_DISPLAY_LIBRARY_GLFW "dynamicLibraries/libdisplay_glfw.so"
#define PATH_DISPLAY_LIBRARY_SDL "dynamicLibraries/libdisplay_sdl.so"

#define PATH_SOUND_LIBRARY_SFML "dynamicLibraries/libsound_sfml.so"
#define PATH_SOUND_LIBRARY_SDL "dynamicLibraries/libsound_sdl.so"

class ServerTCP;

class SnakeServer;
class SnakeClient;

class ClientTCP;

class Core;

namespace KINU {

	class World;
};

class Univers {
public:

	enum eDisplay {
		kExternSfmlLibrary,
		kExternSdlLibrary,
		kExternGlfwLibrary
	};

	enum eFlag {
		SOUND
	};

	Univers();

	bool load_extern_lib_display(eDisplay);

	bool load_external_sound_library(std::string const &library_path); // TODO GO PRIVATE
	void unload_external_library();

	void loop();

	void manage_start();

	void new_game();

	void defaultAssignmentLibrary();

	Core &getCore_() const;

	Core *releaseCore_();

	void refreshTimerLoopWorld();

	void playNoise(eSound e) const;

	void playMusic(std::string const &path) const;

	bool isIASnake(uint16_t client_id) const;

	/** Create && Delete function**/

	void create_ia();

	void create_server(unsigned int port = 4242);

	void create_client();

	void create_ui(); // TODO PRIVATE

	void delete_ia();

	void delete_server();

	void delete_client();

	/** Setter && Getter**/

	const std::unique_ptr<SnakeServer> &getServerTCP_() const;
	MutantGrid<eSprite> &getGrid_();

	//Network
	const std::array<Snake, SNAKE_MAX> getSnakeArray_() const;

	SnakeClient *getSnakeClient() const;

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

	bool isOnlyIA() const;

	bool isServer() const;
	void callbackAction(eAction);
	void manageSwitchLibrary();
private: // Function

	void manage_input();

	void loop_world();

	bool dlError(char const *from);

private:

	//const
	static const std::string WarningServerCreateIA;
	static const std::string WarningServerFull;
	static const std::string WarningServerIsUp;
	static const std::string SuccessServerIsCreate;
	static const std::string WarningClientExist;

	boost::filesystem::path		pathRoot_;

	bool switchLib;
	// Variable
	std::vector<NextFrame> nextFrame;
	std::bitset<32> flag;
	boost::asio::io_service io_loop;
	boost::asio::io_service io_start;
	boost::asio::deadline_timer timer_loop;
	boost::asio::deadline_timer timer_start;
	std::unique_ptr<KINU::World> world_;
	std::unique_ptr<SnakeServer> serverTCP_;
	std::shared_ptr<SnakeClient> clientTCP_;
	std::unique_ptr<Core> core_;
	std::shared_ptr<MutantGrid<eSprite>> grid_;
public:

private:

	std::vector<std::unique_ptr<Bobby>> vecBobby;

	unsigned int mapSize;
	unsigned int gameSpeed;
	void *dlHandleDisplay;
	void *dlHandleSound;
	IDisplay *display;
	ISound *sound;
	bool borderless;
	bool openGame_;
public:
	bool isOpenGame_() const;

	void setOpenGame_(bool openGame_);

private:
	boost::thread thread;
	eDisplay kDisplay;
public:
	virtual ~Univers();

private:

	bool load_external_display_library(std::string const &title,
									   std::string const &libPath);

	void finish_game();
	IDisplay *(*newDisplay)(int, int, char const *);

	ISound *(*newSound)(char const *);

	void (*deleteDisplay)(IDisplay *);

	void (*deleteSound)(ISound *);

};


