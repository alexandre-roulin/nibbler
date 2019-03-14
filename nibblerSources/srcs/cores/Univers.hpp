#pragma once

#include <ia/Bobby.hpp>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/filesystem.hpp>
#include <bitset>
#include "nibbler.hpp"
#include "ISound.hpp"
#include "dynamicLinkLibrary/SoundDynamicLibrary.hpp"
#include "dynamicLinkLibrary/DisplayDynamicLibrary.hpp"
#include "GameManager.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <events/NextFrame.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <cores/GameManager.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_DNS "localhost"
#define DEFAULT_PORT "8000"
#define DEFAULT_PORT_NU 8000

class SnakeServer;

class SnakeClient;

class Gui;

class Univers {

public:

	using BobbyContainer = std::vector<std::unique_ptr<Bobby>>;

	/** Univers **/

	Univers();
	~Univers();
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

	void connect(const std::string dns, const std::string port);

	std::string getIPAddress();
	// create

	void createBobby();

	void createClient();

	void createServer(const std::string ip,  unsigned int port);

	void createGui();

	// delete

	void deleteBobby(SnakeClient::boost_shared_ptr &ptr);

	void deleteClient();

	void disconnectClient();

	void closeServer();

	void deleteServer();

	void deleteGui();

	void postGameDataManagement();

	/** UI **/

	void sendHostname();

	void updateDisplayUI();

	bool displayIsAvailable() const;

	/** Sound **/

	bool loadSound();

	void unloadSound();

	/** Getter && Setter **/


	void setMicroSecDeltaTime(uint32_t microSecDeltaTime);

	uint32_t getMicroSecDeltaTime() const;

	void setExit(bool b);

	bool isExit() const;

	void setGrid_(const std::shared_ptr<MutantGrid<eSprite>> &grid_);

	void setMapSize(unsigned int mapSize_);

	const unsigned int &getMapSize() const;

	void setBorderless(bool borderless);

	const bool &isBorderless() const;

	void setOpenGame_(bool openGame_);

	bool isOpenGame_() const;

	MutantGrid<eSprite> &getGrid_() const;

	std::unique_ptr<Gui> &getGui_();

	std::shared_ptr<SnakeArrayContainer> getSnakeArray_() const;

	boost::weak_ptr<SnakeClient> getSnakeClient() const;

	boost::weak_ptr<SnakeClient> getMySnakeClient() const;

	boost::weak_ptr<SnakeClient> getBobbySnakeClient(int id) const;

	boost::weak_ptr<ISnakeNetwork> getSnakeNetwork() const;

	bool isOnlyIA() const;

	bool isServer() const;

	bool isSwitchLib() const;

	SnakeServer &getSnakeServer() const;

	BobbyContainer &getBobbys();

	IOManager &getIoManager();

	SoundDynamicLibrary &getSoundManager();

	DisplayDynamicLibrary &getDisplayManager();

	GameManager &getGameManager();

	const GameManager::eSpeed &getBaseSpeed() const;

	void setBaseSpeed(const GameManager::eSpeed &baseSpeed);

	void switchBorderless();

	void switchReady();

	void sendOpenGameToServer();

	void updateSizeMap();
private:

	void loadSoundData_();
	static const std::string SuccessServerHasBeenClosed;
	static const std::string SuccessClientIsDisconnect;
	static const std::string SuccessResizeMapTo;
	static const std::string SuccessServerIsCreate;
	static const std::string SuccessClientIsCreate;
	static const std::string SuccessClientIsDelete;
	static const std::string SuccessServerIsDelete;
	static const std::string SuccessIAIsCreate;
	static const std::string SuccessClientIsConnected;
	static const std::string SuccessBorderlessSet;
	static const std::string SuccessBorderlessUnset;

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
	static const std::string ErrorPortRange;
	boost::shared_ptr<IOManager> ioManager;
	std::unique_ptr<SoundDynamicLibrary> soundManager;
	std::unique_ptr<DisplayDynamicLibrary> displayManager;
	std::unique_ptr<GameManager> gameManager;

	boost::shared_ptr<SnakeServer> snakeServer_;
	boost::shared_ptr<SnakeClient> snakeClient_;
	std::shared_ptr<MutantGrid<eSprite>> grid_;
	std::unique_ptr<Gui> gui_;
	BobbyContainer vecBobby;

	bool exit_;
	bool switchLib_;
	unsigned int mapSize_;
	uint32_t microSecDeltaTime_;
	GameManager::eSpeed baseSpeed;
	bool borderless_;
	bool openGame_;
	eDirection lastDirection_;
};


