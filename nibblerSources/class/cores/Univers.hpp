#pragma once

#include <ia/Bobby.hpp>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/filesystem.hpp>
#include <bitset>
#include "nibbler.hpp"
#include "ISound.hpp"
#include "ExternalLibrarySoundManager.hpp"
#include "ExternalLibraryDisplayManager.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <events/NextFrame.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>

#define DEFAULT_DNS "localhost"
#define DEFAULT_PORT "4242"

class GameManager;

class SnakeServer;

class SnakeClient;

class Gui;

class Univers {

public:

	using BobbyContainer = std::vector<std::unique_ptr<Bobby>>;

	/** Univers **/

	Univers();

	virtual ~Univers();

	Univers &operator=(const Univers &) = delete;

	Univers(const Univers &) = delete;

	void resetData();

	void callbackAction(eAction);

	/** Game **/

	void startNewGame();

	void manageSnakeClientInput();

	/** Library **/

	void defaultAssignmentLibrary();

	void manageSwitchLibrary();

	/** Snake **/

	bool isIASnake(uint16_t client_id) const;

	/** Network **/

	void connect(std::string const &dns = DEFAULT_DNS,
				 std::string const &port = DEFAULT_PORT);


	// create

	void createBobby();

	void createClient();

	void createServer(unsigned int port = 4242);

	void createGui();

	// delete

	void deleteBobby(int16_t id);

	void deleteClient();

	void deleteServer();

	void deleteGui();

	/** UI **/

	void sendHostname();

	void updateDisplayUI();

	bool displayIsAvailable() const;

	/** Getter && Setter **/

	void setMicroSecDeltaTime(uint32_t microSecDeltaTime);

	uint32_t getMicroSecDeltaTime() const;

	void setExit(bool b);

	bool isExit() const;

	void setGrid_(const std::shared_ptr<MutantGrid<eSprite>> &grid_);

	void setMapSize(unsigned int mapSize_);

	unsigned int getMapSize() const;

	void setBorderless(bool borderless);

	bool isBorderless() const;

	void setOpenGame_(bool openGame_);

	bool isOpenGame_() const;

	MutantGrid<eSprite> &getGrid_() const;

	std::unique_ptr<Gui> &getGui_();

	const SnakeArrayContainer &getSnakeArray_() const;

	boost::weak_ptr<SnakeClient> getSnakeClient() const;

	KINU::World &getWorld_();

	bool isOnlyIA() const;

	bool isServer() const;

	bool isSwitchLib() const;

	SnakeServer &getSnakeServer() const;

	BobbyContainer &getBobbys();

	IOManager &getIoManager();

	ExternalLibrarySoundManager &getSoundManager();

	GameManager &getGameManager();

private:

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
	static const std::string WarningAllClientIsNotReady;

	static const std::string WarningUserIsNotTheServer;
	static const std::string WarningRequiredAtLeastOneClient;

	static const std::string ErrorClientConnectionRefused;
	static const std::string ErrorServerAlreadyUseOnThisPort;


	std::unique_ptr<IOManager> ioManager;
	std::unique_ptr<ExternalLibrarySoundManager> soundManager;
	std::unique_ptr<ExternalLibraryDisplayManager> displayManager;
	std::unique_ptr<GameManager> gameManager;

	boost::shared_ptr<SnakeServer> snakeServer_;
	boost::shared_ptr<SnakeClient> snakeClient_;
	std::shared_ptr<MutantGrid<eSprite>> grid_;
	std::unique_ptr<Gui> gui_;
	BobbyContainer vecBobby;

	bool exit_;
	bool switchLib;
	unsigned int mapSize_;
	uint32_t microSecDeltaTime;
	bool borderless;
	bool openGame_;
	SnakeArrayContainer snakeArrayContainer;

};


