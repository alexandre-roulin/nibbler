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
#include <boost/scoped_ptr.hpp>
#define PATH_DISPLAY_LIBRARY_SFML "dynamicLibraries/libdisplay_sfml.so"
#define PATH_DISPLAY_LIBRARY_GLFW "dynamicLibraries/libdisplay_glfw.so"
#define PATH_DISPLAY_LIBRARY_SDL "dynamicLibraries/libdisplay_sdl.so"

#define LOCALHOST "localhost"
#define PATH_SOUND_LIBRARY_SFML "dynamicLibraries/libsound_sfml.so"
#define PATH_SOUND_LIBRARY_SDL "dynamicLibraries/libsound_sdl.so"

class SnakeServer;

class SnakeClient;

class ClientTCP;

class Gui;

namespace KINU {

	class World;
};

class Univers {
public:

	enum eDisplay {
		kDisplaySfmlLibrary,
		kDisplaySdlLibrary,
		kDisplayGlfwLibrary
	};

	enum eSound {
		kSoundSfmlLibrary,
		kSoundSdlLibrary
	};

	enum eFlag {
		kSound
	};

	Univers();

	bool load_extern_lib_display(eDisplay);

	bool load_extern_lib_sound(eSound eLib);

	void unload_external_sound_library();

	void unload_external_display_library();

	void loop();

	void manage_start();

	void new_game();

	void defaultAssignmentLibrary();

	void sendHostname();

	void refreshTimerLoopWorld();

	void addNoise(std::string const &path);

	void playNoise(eNoise e) const;

	void playMusic(std::string const &path) const;

	bool isIASnake(uint16_t client_id) const;

	bool isExit() const;
	void setExit(bool b);

	void connect(std::string const &dns = "localhost",
				 std::string const &port = "4242");

	/** Create && Delete function**/

	void create_ia();

	void create_server(unsigned int port = 4242);

	void create_client();

	void delete_ia(int16_t id);

	void delete_ia();

	void deleteServer();

	void deleteClient();

	void createGui();
	void deleteGui();

	std::unique_ptr<Gui> &getGui_();

	/** Setter && Getter**/

	MutantGrid<eSprite> &getGrid_();

	//Network
	std::array<Snake, SNAKE_MAX> getSnakeArray_() const;

	boost::shared_ptr<SnakeClient> getSnakeClient() const;

	//Game

	KINU::World &getWorld_() const;

	void setMapSize(unsigned int mapSize_);

	bool isBorderless() const;

	void setBorderless(bool borderless);

	unsigned int getMapSize() const;


	//Sound

	ISound &getSound() const;

	//State

	bool isOnlyIA() const;

	bool isServer() const;

	void callbackAction(eAction);


	virtual ~Univers();

	bool isOpenGame_() const;

	void setOpenGame_(bool openGame_);

private: // Function
	void manageSwitchLibrary();

	void cleanAll();

	void manage_input();

	void loop_world();

	bool dlError(char const *from);

private:

	//const

	static const std::string SuccessServerIsCreate;
	static const std::string SuccessClientIsCreate;
	static const std::string SuccessClientIsDelete;
	static const std::string SuccessServerIsDelete;
	static const std::string SuccessIAIsCreate;
	static const std::string SuccessClientIsConnected;

	static const std::string WarningServerCreateIA;
	static const std::string WarningServerRemoveIA;
	static const std::string WarningServerFull;

	static const std::string WarningServerExist;
	static const std::string WarningClientExist;

	static const std::string WarningClientNotExist;
	static const std::string WarningServerNotExist;

	static const std::string WarningClientIsAlreadyConnected;
	static const std::string WarningClientIsNotConnected;
	static const std::string WarningClientConnectError;

	static const std::string WarningUserIsNotTheServer;
	static const std::string WarningRequiredAtLeastOneClient;

	static const std::string ErrorClientConnectionRefused;
	static const std::string ErrorServerAlreadyUseOnThisPort;
	// Variable
	boost::filesystem::path pathRoot_;
	std::bitset<32> flag_;
	bool exit_;
	bool switchLib;
	std::vector<NextFrame> nextFrame;
	boost::asio::io_service io_loop;
	boost::asio::io_service io_start;
	boost::asio::deadline_timer timer_loop;
	boost::asio::deadline_timer timer_start;
	std::shared_ptr<KINU::World> world_;

	boost::shared_ptr<SnakeServer> snakeServer_;
	boost::shared_ptr<SnakeClient> snakeClient_;
	std::unique_ptr<Gui> gui_;
	std::shared_ptr<MutantGrid<eSprite>> grid_;
	std::vector<std::unique_ptr<Bobby>> vecBobby;

	unsigned int mapSize_;
	unsigned int gameSpeed;
	void *dlHandleDisplay;
	void *dlHandleSound;
	IDisplay *display;
	ISound *sound;
	bool borderless;
	bool openGame_;


	boost::thread thread;
	eDisplay kDisplay;

	bool load_external_display_library(std::string const &title,
									   std::string const &libPath);
	bool load_external_sound_library(std::string const &library_path);

	void finish_game();

	IDisplay *(*newDisplay)(int, int, char const *);

	ISound *(*newSound)(char const *);

	void (*deleteDisplay)(IDisplay *);

	void (*deleteSound)(ISound *);

};


