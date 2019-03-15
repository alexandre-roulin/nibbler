#include "Univers.hpp"
#include <gui/Gui.hpp>
#include <KINU/World.hpp>
#include <network/SnakeServer.hpp>
#include "GameManager.hpp"
#include "Snake.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

/** Const Variable **/
const std::string Univers::SuccessServerHasBeenClosed = "Server now refused connection.";
const std::string Univers::SuccessServerIsCreate = "Server is online on port ";
const std::string Univers::SuccessIAIsCreate = "IA is online.";
const std::string Univers::SuccessClientIsConnected = "Client is connected.";
const std::string Univers::SuccessClientIsCreate = "Client is online";
const std::string Univers::SuccessClientIsDelete = "Client is delete.";
const std::string Univers::SuccessServerIsDelete = "Server is delete.";
const std::string Univers::SuccessBorderlessSet = "Borderless is set.";
const std::string Univers::SuccessBorderlessUnset = "Borderless is unset.";
const std::string Univers::SuccessClientIsDisconnect = "Client is disconnect.";
const std::string Univers::SuccessResizeMapTo = "Map has been resize to ";

const std::string Univers::WarningClientExist = "Client is already in place.";
const std::string Univers::WarningServerCreateIA = "Only the server owner can create IA.";
const std::string Univers::WarningServerRemoveIA = "Only the server owner can remove IA.";
const std::string Univers::WarningServerFull = "Server have reach the maximum player.";
const std::string Univers::WarningServerExist = "Server is left online.";
const std::string Univers::WarningClientNotExist = "There is no client online.";
const std::string Univers::WarningServerNotExist = "There is no server online.";
const std::string Univers::WarningClientIsAlreadyConnected = "Client is already connected.";
const std::string Univers::WarningClientIsNotConnected = "Client is not connected.";
const std::string Univers::WarningAllClientIsNotReady = "Client are not ready.";
const std::string Univers::WarningUserIsNotTheServer = "User is not the server.";
const std::string Univers::WarningRequiredAtLeastOneClient = "You need to have at least one client to start game.";


const std::string Univers::ErrorClientConnectionRefused = "Connection refused.";
const std::string Univers::ErrorServerAlreadyUseOnThisPort = "Server already in use on this port.";
const std::string Univers::ErrorPortRange = "Port should be between 1024 and 65545.";
/** Univers **/

Univers::Univers()
		:
		ioManager(std::make_unique<IOManager>()),
		soundManager(std::make_unique<SoundDynamicLibrary>()),
		displayManager(std::make_unique<DisplayDynamicLibrary>()),
		gameManager(std::make_unique<GameManager>(*this)),
		snakeServer_(nullptr),
		snakeClient_(nullptr),
		grid_(nullptr),
		gui_(nullptr),
		exit_(false),
		switchLib_(false),
		mapSize_(MAP_DEFAULT),
		microSecDeltaTime_(0),
		baseSpeed(GameManager::eSpeed::Medium),
		borderless_(false),
		openGame_(false),
		lastDirection_(kNorth) {
}

void Univers::resetData() {
	switchLib_ = false;
	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());
	if (ptr && !ptr->isOpen()) {
		deleteClient();
		borderless_ = false;
		mapSize_ = MAP_DEFAULT;
	}
	grid_ = nullptr;
}

void Univers::callbackAction(eAction action) {
	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());
	boost::shared_ptr<ISnakeNetwork> ptr_network(getSnakeNetwork().lock());

	switch (action) {
		case eAction::kSwitchDisplayLibrarySFML:
			if (eDisplay::kDisplaySfmlLibrary == displayManager->getKInstance())
				return;
			displayManager->setNextKInstance(eDisplay::kDisplaySfmlLibrary);
			switchLib_ = true;
			break;
		case eAction::kSwitchDisplayLibrarySDL:
			if (eDisplay::kDisplaySdlLibrary == displayManager->getKInstance())
				return;
			displayManager->setNextKInstance(eDisplay::kDisplaySdlLibrary);
			switchLib_ = true;
			break;
		case eAction::kSwitchDisplayLibraryGLFW:
			if (eDisplay::kDisplayGlfwLibrary == displayManager->getKInstance())
				return;
			displayManager->setNextKInstance(eDisplay::kDisplayGlfwLibrary);
			switchLib_ = true;
			break;
		case eAction::kPause :
			ptr->sendDataToServer(action, eHeader::kPause);
			break;
		case eAction::kSwitchDisplayLibrary:
			displayManager->setNextKInstance();
			switchLib_ = true;
			break;
	}
}


Univers::~Univers() {
	if (snakeServer_) {
		snakeServer_->closeAcceptorServer();
		snakeServer_ = nullptr;
	}
	std::for_each(bobbyContainer.begin(), bobbyContainer.end(), [](std::unique_ptr<Bobby> &bobby){
		if (bobby) {
			bobby->getClientTCP_()->disconnect();
			bobby = nullptr;
		}
	});
	if (snakeClient_) {
		snakeClient_->disconnect();
		snakeClient_ = nullptr;
	}
	ioManager = nullptr;
};

/** Game **/

void Univers::startNewGame() {

	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());
	SnakeServer::b_ptr ptrServer(snakeServer_);
	setMicroSecDeltaTime(static_cast<uint32_t >(baseSpeed));

	try {
		displayManager->loadDynamicConstructor();
		deleteGui();
		defaultAssignmentLibrary();
	} catch (std::exception const &e) {
		if (gui_)
			gui_->addMessageChat(eColorLog::kRed, e.what());
		postGameDataManagement();
		return;
	}

	gameManager->startNewGame();
	gameManager->loopUI();

	postGameDataManagement();
}

void Univers::manageSnakeClientInput() {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);
	eDirection direction = lastDirection_;
	if (displayManager->hasLibraryLoaded()) {
		direction = displayManager->getInstance()->getDirection();
		lastDirection_ = direction;
	}

	if (ptr && ptr->isOpen() && !ptr->isIa()) {
		ptr->sendDirection(direction);
	}
}


/** Library **/

void Univers::postGameDataManagement() {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);
	SnakeServer::b_ptr ptrServer(snakeServer_);

	if (ptr) {
		ptr->quitGame();
	}
	if (ptrServer) {
		std::for_each(bobbyContainer.begin(), bobbyContainer.end(), [](std::unique_ptr<Bobby> &bob){
			bob->getClientTCP_()->quitGame();
		});
		ptrServer->sendOpenGameToClient(false);
	}
	openGame_ = false;
	gameManager->finishGame();
	if (displayManager->hasLibraryLoaded())
		displayManager->unloadDynamicLibrary();
	resetData();
}

void Univers::defaultAssignmentLibrary() {

	MutantGrid<eSprite> grid(mapSize_);
	grid.fill(eSprite::kGround);
	SnakeClient::boost_shared_ptr client(snakeClient_);
	
	try {
		displayManager->loadDynamicLibrary(
				mapSize_, mapSize_,
				displayManager->getNextLibraryInfo().title.c_str(),
				(client ? client->getSnake().direction : eDirection::kNorth));
		displayManager->getInstance()->setBackground(grid);
		displayManager->getInstance()->registerCallbackAction(std::bind(&Univers::callbackAction, this, std::placeholders::_1));
	} catch (std::exception const &e){
		throw (std::runtime_error("Cant load IDisplay"));
	}
}

void Univers::manageSwitchLibrary() {
	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());
	if (!ptr->isSwitchingLibrary()) {
		int16_t id = ptr->getId_();
		ptr->sendDataToServer(id, eHeader::kForcePause);
		try {
			displayManager->loadDynamicConstructor();
			defaultAssignmentLibrary();
		} catch (const DisplayDynamicLibrary::Error &ae){
			ptr->sendDataToServer(id, eHeader::kForcePause);
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
			postGameDataManagement();
			displayManager->unloadDynamicLibrary();
			boost::asio::post(ioManager->getIo(), [this, ae](){
				while (gui_ == nullptr);
				std::cout << (gui_ == nullptr) << std::endl;
				if (gui_)
					gui_->addMessageChat(eColorLog::kRed, ae.what());

			});
		} catch (std::exception const &e) {
			ptr->sendDataToServer(id, eHeader::kForcePause);
			boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
			postGameDataManagement();
			displayManager->unloadDynamicLibrary();
			boost::asio::post(ioManager->getIo(), [this, e](){
				while (gui_ == nullptr);
				std::cout << (gui_ == nullptr) << std::endl;
				if (gui_)
					gui_->addMessageChat(eColorLog::kRed, e.what());

			});
		}
		ptr->sendDataToServer(id, eHeader::kForcePause);
	}
	switchLib_ = false;
}

/** Snake **/

bool Univers::isIASnake(uint16_t client_id) const {
	return std::any_of(bobbyContainer.begin(), bobbyContainer.end(), [client_id](auto &bobby){ return bobby->getId() == client_id;});
}

/** Network **/


void Univers::connect(const std::string dns, const std::string port) {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (!ptr) {
		gui_->addMessageChat(eColorLog::kOrange, WarningClientNotExist);
		return;
	}
	if (ptr->isOpen()) {
		gui_->addMessageChat(eColorLog::kOrange,
							 WarningClientIsAlreadyConnected);
		return;
	}
	try {
		ptr->connect(dns, port);
		if (ptr->isOpen() && gui_)
			gui_->addMessageChat(eColorLog::kGreen, SuccessClientIsConnected);
		else if (gui_)
			gui_->addMessageChat(eColorLog::kOrange,
								 WarningClientIsNotConnected);
	} catch (const boost::system::system_error &ex) {
		if (ex.code() == boost::system::errc::connection_refused && gui_) {
			gui_->addMessageChat(eColorLog::kRed, ErrorClientConnectionRefused);
		} else if (gui_) {
			gui_->addMessageChat(eColorLog::kRed, ex.what());
		}
	} catch (const std::exception &e) {
		if (gui_)
			gui_->addMessageChat(eColorLog::kRed, e.what());
	}
}
void Univers::createServer(const std::string dns, unsigned int port) {

	if (snakeServer_ && gui_)
		gui_->addMessageChat(eColorLog::kOrange, WarningServerExist);
	else if (port >> 16 != 0 && gui_) {
		gui_->addMessageChat(eColorLog::kRed, ErrorPortRange);
	}
	else {
		try {
			snakeServer_ = SnakeServer::create(*this, dns, port);
			if (gui_)
			gui_->addMessageChat(eColorLog::kGreen, SuccessServerIsCreate + std::to_string(snakeServer_->getPort_()));
		} catch (const boost::system::system_error &ex) {
			if (boost::system::errc::address_in_use == ex.code() && gui_) {
				gui_->addMessageChat(eColorLog::kRed, ErrorServerAlreadyUseOnThisPort);
			} else if (gui_) {
				gui_->addMessageChat(eColorLog::kRed, ex.what());
			}
		} catch (std::exception const &e) {
			if (gui_)
				gui_->addMessageChat(eColorLog::kRed, e.what());
		}

	}


}

void Univers::createBobby() {
	if (!isServer() && gui_) {
		gui_->addMessageChat(eColorLog::kOrange, WarningServerCreateIA);
		return;
	}
	if (snakeServer_->isFull() && gui_) {
		gui_->addMessageChat(eColorLog::kOrange, WarningServerFull);
		return;
	}
	std::unique_ptr<Bobby> bobby = std::make_unique<Bobby>(*this);
	try {
		bobby->getClientTCP_()->connect(getSnakeServer().getAddress_(), std::to_string(snakeServer_->getPort_()));
	} catch (const std::exception &e) {
		if (gui_)
			gui_->addMessageChat(e.what());
	}
	bobbyContainer.push_back(std::move(bobby));
	if (gui_)
		gui_->addMessageChat(eColorLog::kGreen, SuccessIAIsCreate);
}

void Univers::createClient() {

	if (!snakeClient_) {
		snakeClient_ = SnakeClient::create(*this, false);
		if (gui_)
			gui_->addMessageChat(eColorLog::kGreen, SuccessClientIsCreate);
	} else if (gui_) {
		gui_->addMessageChat(eColorLog::kOrange, WarningClientExist);
	}

}

void Univers::createGui() {
	gui_ = std::make_unique<Gui>(*this);
}


void Univers::disconnectClient() {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (ptr) {
		ptr->disconnect();
		if (gui_) gui_->addMessageChat(eColorLog::kGreen, SuccessClientIsDisconnect);
	} else if (gui_)
		gui_->addMessageChat(eColorLog::kOrange, WarningClientNotExist);
}

void Univers::closeServer() {
	boost::shared_ptr<SnakeServer> ptr(snakeServer_);

	if (ptr) {
		ptr->closeAcceptorServer();
		if (gui_) gui_->addMessageChat(eColorLog::kOrange, SuccessServerHasBeenClosed);
	} else {
		if (gui_) gui_->addMessageChat(eColorLog::kOrange, WarningServerNotExist);
	}
}

void Univers::deleteBobby(SnakeClient::boost_shared_ptr &ptr) {
	if (!isServer()) {
		gui_->addMessageChat(eColorLog::kOrange, WarningServerRemoveIA);
		return ;
	}

	ptr->disconnect();
	bobbyContainer.erase(std::remove_if(bobbyContainer.begin(), bobbyContainer.end(),
			[ptr](std::unique_ptr<Bobby> &bob) {
		return bob->getId() == ptr->getId_();
	}));
}

void Univers::deleteClient() {

	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (ptr) {
		snakeClient_->disconnect();
		if (gui_) gui_->addMessageChat(eColorLog::kGreen, SuccessClientIsDelete);
		snakeClient_ = nullptr;
	} else if (gui_)
		gui_->addMessageChat(eColorLog::kOrange, WarningClientNotExist);
}

void Univers::deleteServer() {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);
	SnakeServer::b_ptr ptrServer(snakeServer_);

	if (ptrServer) {
		snakeServer_ = nullptr;
		if (gui_)
			gui_->addMessageChat(eColorLog::kGreen, SuccessServerIsDelete);
		std::for_each(bobbyContainer.begin(), bobbyContainer.end(), [](std::unique_ptr<Bobby> &bob){ bob->getClientTCP_()->disconnect(); });
		bobbyContainer.clear();
		if (ptr && ptr->isOpen())
			disconnectClient();
	} else if (gui_) {
			gui_->addMessageChat(eColorLog::kOrange, WarningServerNotExist);
	}
}

void Univers::deleteGui() {
	gui_.reset(nullptr);
}

/** Sound **/

void Univers::loadSoundData_() {
	boost::filesystem::path pathSound(NIBBLER_ROOT_PROJECT_PATH);
	pathSound = pathSound / "ressources" / "sound";

	getSoundManager().addNoise((pathSound / "appear-online.ogg").generic_string());
	getSoundManager().addNoise((pathSound / "yes-2.wav").generic_string());
	getSoundManager().addNoise((pathSound / "click.wav").generic_string());
	getSoundManager().addNoise((pathSound / "slime10.wav").generic_string());
	getSoundManager().addNoise((pathSound / "hit17.ogg").generic_string());
	getSoundManager().addNoise((pathSound / "gogogogo-mp3cut.ogg").generic_string());

}

bool Univers::loadSound() {
	getSoundManager().loadDynamicConstructor();
	getSoundManager().loadDynamicLibrary();
	if (!getSoundManager().hasLibraryLoaded())
		return false;
	loadSoundData_();
	return true;
}
void Univers::unloadSound() {
	getSoundManager().unloadDynamicLibrary();
}

/** Getter && Setter **/

uint32_t Univers::getMicroSecDeltaTime() const {
	return microSecDeltaTime_;
}

void Univers::setMicroSecDeltaTime(uint32_t microSecDeltaTime) {
	if (microSecDeltaTime < GameManager::Impossible || microSecDeltaTime > GameManager::Easy)
		microSecDeltaTime_ = GameManager::Impossible;
	else
		microSecDeltaTime_ = microSecDeltaTime;
}

bool Univers::isExit() const {
	return exit_;
}

void Univers::setExit(bool b) {
	exit_ = b;
}

std::unique_ptr<Gui> &Univers::getGui_() {
	return gui_;
}

MutantGrid<eSprite> &Univers::getGrid_() const {
	return *grid_;
}
std::shared_ptr<SnakeArrayContainer> Univers::getSnakeArray_() const {

	boost::shared_ptr<ISnakeNetwork>  ptr(getSnakeNetwork().lock());

	if (ptr) {
		return ptr->getSnakeArray_();
	}
 	return std::shared_ptr<SnakeArrayContainer>(nullptr);
}

boost::weak_ptr<ISnakeNetwork> Univers::getSnakeNetwork() const {

	SnakeClient::boost_shared_ptr ptr(snakeClient_);
	SnakeServer::b_ptr ptrServer(snakeServer_);

	if (ptr && ptr->isOpen())
		return ptr->shared_from_this();
	else if (bobbyContainer.size() != 0)
		return bobbyContainer.front()->getClientTCP_()->shared_from_this();
	else if (isServer())
		return ptrServer->shared_from_this();
	return boost::weak_ptr<SnakeClient>();
}

boost::weak_ptr<SnakeClient> Univers::getSnakeClient() const {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (ptr && ((openGame_ && ptr->isOpen()) || !openGame_))
		return ptr->shared_from_this();
	else if (bobbyContainer.size() != 0)
		return bobbyContainer.front()->getClientTCP_()->shared_from_this();
	return boost::weak_ptr<SnakeClient>();
}

boost::weak_ptr<SnakeClient> Univers::getMySnakeClient() const {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (ptr && ((openGame_ && ptr->isOpen()) || !openGame_))
		return ptr->shared_from_this();
	return boost::weak_ptr<SnakeClient>();
}

boost::weak_ptr<SnakeClient> Univers::getBobbySnakeClient(int id) const {
	auto itBobby = std::find_if(bobbyContainer.begin(), bobbyContainer.end(), [id](std::unique_ptr<Bobby> const &bob){ return bob->getId() == id; });
	if (itBobby == bobbyContainer.end())
		return boost::weak_ptr<SnakeClient>();
	return (*itBobby)->getClientTCP_()->shared_from_this();
}

const unsigned int &Univers::getMapSize() const {
	return mapSize_;
}

void Univers::setMapSize(unsigned int mapSize) {
	mapSize_ = mapSize;
}

void Univers::setBorderless(bool borderless) {
	Univers::borderless_ = borderless;
}

bool Univers::isServer() const {
	return snakeServer_ != nullptr;
}

const bool &Univers::isBorderless() const {
	return borderless_;
}

bool Univers::isOnlyIA() const {
	return (snakeClient_ == nullptr || !snakeClient_->isOpen()) && bobbyContainer.size() != 0;
}


bool Univers::isOpenGame_() const {
	return openGame_;
}

void Univers::setOpenGame_(bool openGame) {
	Univers::openGame_ = openGame;
}

void Univers::sendHostname() {
	if (gui_) {
		std::stringstream ss;
		std::string s;
		char hostname[64];
		gethostname(hostname, 64);
		gui_->addMessageChat(std::string("[ Hostname ] ") + hostname);
		gui_->addMessageChat(std::string("[ Local IP Address ] ") + getIPAddress());
		std::system("curl \"http://myexternalip.com/raw\" 2>&- > /tmp/external_ip");
		ss << std::ifstream("/tmp/external_ip").rdbuf();
		ss >> s;
		gui_->addMessageChat(std::string("[ Extern IP Address ] ") + s);
	}
}

IOManager &Univers::getIoManager() {
	return *ioManager;
}

void Univers::setGrid_(const std::shared_ptr<MutantGrid<eSprite>> &grid_) {
	Univers::grid_ = grid_;
}

bool Univers::displayIsAvailable() const {
	return !displayManager->hasLibraryLoaded() ||
		!displayManager->getInstance()->exit();
}

bool Univers::isSwitchLib() const {
	return switchLib_;
}

void Univers::updateDisplayUI() {
	assert(displayManager != nullptr);
	assert(displayManager->getInstance() != nullptr);

	displayManager->getInstance()->update();
	displayManager->getInstance()->drawGrid(*grid_);
	displayManager->getInstance()->render();
}

SoundDynamicLibrary &Univers::getSoundManager() {
	return *soundManager;
}

DisplayDynamicLibrary &Univers::getDisplayManager() {
	return (*displayManager);
}

Univers::BobbyContainer &Univers::getBobbys() {
	return bobbyContainer;
}

GameManager &Univers::getGameManager() {
	return *gameManager;
}

void Univers::switchBorderless() {
	boost::shared_ptr<ISnakeNetwork> ptr_network(getSnakeNetwork().lock());

	setBorderless(!isBorderless());

	if (isBorderless())
		gui_->addMessageChat(eColorLog::kGreen, SuccessBorderlessSet);
	else
		gui_->addMessageChat(eColorLog::kGreen, SuccessBorderlessUnset);
	if (ptr_network) {
		ptr_network->notifyBorderless();
	}
}

void Univers::switchReady() {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (!ptr) {
		gui_->addMessageChat(eColorLog::kOrange, WarningClientNotExist);
		return;
	}
	if (!ptr->isOpen()) {
		gui_->addMessageChat(eColorLog::kOrange, WarningClientIsNotConnected);
		return;
	}
	ptr->changeStateReady(!ptr->isReady());
}

void Univers::sendOpenGameToServer() {
	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());
	SnakeServer::b_ptr ptrServer(snakeServer_);

	if (!isServer()) {
		gui_->addMessageChat(eColorLog::kOrange,
							 WarningUserIsNotTheServer);
		return;
	}
	if (!ptr) {
		gui_->addMessageChat(eColorLog::kOrange, WarningRequiredAtLeastOneClient);
		return;
	}
	if (!ptrServer->isReady()) {
		gui_->addMessageChat(eColorLog::kOrange, WarningAllClientIsNotReady);
		return;
	}
	ptrServer->sendOpenGameToClient();
}

void Univers::updateSizeMap() {
	boost::shared_ptr<ISnakeNetwork> ptr_network(getSnakeNetwork().lock());

	gui_->addMessageChat(eColorLog::kGreen, SuccessResizeMapTo + std::to_string(mapSize_));
	if (ptr_network)
		ptr_network->notifyMapSize();
}

SnakeServer &Univers::getSnakeServer() const {
	return *snakeServer_;
}

const GameManager::eSpeed &Univers::getBaseSpeed() const {
	return baseSpeed;
}

void Univers::setBaseSpeed(const GameManager::eSpeed &baseSpeed) {
	if (baseSpeed == GameManager::eSpeed::Easy ||
	baseSpeed == GameManager::eSpeed::Medium ||
	baseSpeed == GameManager::eSpeed::Hard ||
	baseSpeed == GameManager::eSpeed::Impossible)
		Univers::baseSpeed = baseSpeed;
}

std::string Univers::getIPAddress() {
	std::string ipAddress = "localhost";
	struct ifaddrs *interfaces = nullptr;
	struct ifaddrs *temp_addr = nullptr;
	int success = 0;

	success = getifaddrs(&interfaces);

	if (success == 0) {
		temp_addr = interfaces;
		while ( temp_addr != NULL ) {
			if ( temp_addr->ifa_addr->sa_family == AF_INET ) {
				if ( strcmp(temp_addr->ifa_name, "en0") == 0 ){
					ipAddress = inet_ntoa((reinterpret_cast<struct sockaddr_in*>(temp_addr->ifa_addr))->sin_addr);
				}
			}
			temp_addr = temp_addr->ifa_next;
		}
	}
	freeifaddrs(interfaces);
	return ipAddress;
}
