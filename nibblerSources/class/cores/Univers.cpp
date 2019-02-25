#include "Univers.hpp"
#include <gui/Gui.hpp>
#include <KINU/World.hpp>
#include <network/SnakeServer.hpp>
#include "GameManager.hpp"

/** Const Variable **/

const std::string Univers::SuccessServerIsCreate = "Server is online.";
const std::string Univers::SuccessIAIsCreate = "IA is online.";
const std::string Univers::SuccessClientIsConnected = "Client is connected.";
const std::string Univers::SuccessClientIsCreate = "Client is online";
const std::string Univers::SuccessClientIsDelete = "Client is delete.";
const std::string Univers::SuccessServerIsDelete = "Server is delete.";
const std::string Univers::SuccessBorderlessSet = "Borderless is set.";
const std::string Univers::SuccessBorderlessUnset = "Borderless is unset.";

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

/** Univers **/

Univers::Univers()
		:
		ioManager(std::make_unique<IOManager>(10)),
		soundManager(std::make_unique<ExternalLibrarySoundManager>()),
		displayManager(std::make_unique<ExternalLibraryDisplayManager>()),
		gameManager(std::make_unique<GameManager>(*this)),
		snakeServer_(nullptr),
		snakeClient_(nullptr),
		grid_(nullptr),
		gui_(nullptr),
		exit_(false),
		switchLib(false),
		mapSize_(MAP_DEFAULT),
		microSecDeltaTime(GameManager::Hard),
		borderless(false),
		openGame_(false) {

}

void Univers::resetData() {
	for (const auto &snake: getSnakeArray_()) {
		if (snake.isValid) {
			std::cout << "ID:" << snake.id_ << " " << " Score : " << snake.score_ << std::endl;
		}
	}

	openGame_ = false;
	switchLib = false;
	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());
	if (ptr && !ptr->isOpen()) {
		deleteClient();
		borderless = false;
		mapSize_ = MAP_DEFAULT;
	}
	grid_ = nullptr;
}

void Univers::callbackAction(eAction action) {
	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());
	boost::shared_ptr<ISnakeNetwork> ptr_network(getSnakeNetwork().lock());

	switch (action) {
		case eAction::kPause :
			log_success(" eAction::kPause");
			ptr->sendDataToServer(action, eHeader::kPause);
			break;
		case eAction::kSwitchDisplayLibrary:
			log_success("eAction::kSwitchDisplayLibrary");
			switchLib = true;
			break;
	}
}


Univers::~Univers() = default;

/** Game **/

void Univers::startNewGame() {
	if (!displayManager->hasLibraryLoaded())
		displayManager->loadExternalDisplayLibrary(displayManager->getKDisplay());

	defaultAssignmentLibrary();

	gameManager->startNewGame();
	gameManager->loopUI();
	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());

	if (ptr && !ptr->isIa())
		ptr->killSnake(ptr->getId_());
	gameManager->finishGame();
	if (displayManager->hasLibraryLoaded())
		displayManager->unloadExternalDisplayLibrary();
	resetData();
}

void Univers::manageSnakeClientInput() {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);
	log_info("%s snakeClient[%d] Lib[%d] isAlive[%d]", __PRETTY_FUNCTION__, snakeClient_ != nullptr, displayManager->hasLibraryLoaded(), (ptr && ptr->getSnake().isAlive));
	eDirection direction = eDirection::kNorth;
	if (displayManager->hasLibraryLoaded())
		direction = displayManager->getDisplay()->getDirection();
	if (ptr && ptr->isOpen() && ptr->getSnake().isAlive && !isIASnake(ptr->getId_())) {
		ptr->addScore(ptr->getId_(), eScore::kFromTime);
		ptr->sendDataToServer(InputInfo(ptr->getId_(), direction),
							  eHeader::kInput);
	}

}


/** Library **/


void Univers::defaultAssignmentLibrary() {

	MutantGrid<eSprite> grid(mapSize_);
	grid.fill(eSprite::kGround);

	displayManager->constructExternalLibrary(mapSize_,mapSize_);
	displayManager->getDisplay()->setBackground(grid);
	displayManager->getDisplay()->registerCallbackAction(
			std::bind(&Univers::callbackAction, this, std::placeholders::_1));
	displayManager->getDisplay()->update();
	displayManager->getDisplay()->render();
}


void Univers::manageSwitchLibrary() {
	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());
	log_success("sw %d", !ptr->isSwitchingLibrary());
	if (!ptr->isSwitchingLibrary()) {
		int16_t id = ptr->getId_();
		ptr->sendDataToServer(id, eHeader::kForcePause);
		displayManager->switchNextLibrary();
		defaultAssignmentLibrary();
		ptr->sendDataToServer(id, eHeader::kForcePause);
	}
	switchLib = false;
}

/** Snake **/

bool Univers::isIASnake(uint16_t client_id) const {

	return std::any_of(vecBobby.begin(), vecBobby.end(), [client_id](auto &bobby){ return bobby->getId() == client_id;});
}

/** Network **/


void Univers::connect(const std::string dns, const std::string port) {
	log_info("%s", __PRETTY_FUNCTION__);
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (ptr == nullptr) {
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
		if (ptr->isOpen())
			gui_->addMessageChat(eColorLog::kGreen, SuccessClientIsConnected);
		else
			gui_->addMessageChat(eColorLog::kOrange,
								 WarningClientIsNotConnected);
	} catch (const boost::system::system_error &ex) {
		if (ex.code() == boost::system::errc::connection_refused) {
			gui_->addMessageChat(eColorLog::kRed, ErrorClientConnectionRefused);
		} else {
			gui_->addMessageChat(ex.what());
		}
	} catch (const std::exception &e) {
		gui_->addMessageChat(e.what());
	}
}
void Univers::createServer(const std::string, unsigned int port) {
	if (snakeServer_)
		gui_->addMessageChat(eColorLog::kOrange, WarningServerExist);
	else {
		try {
			snakeServer_ = SnakeServer::create(*this, port);
			gui_->addMessageChat(eColorLog::kGreen, SuccessServerIsCreate);
		} catch (const boost::system::system_error &ex) {
			if (boost::system::errc::address_in_use == ex.code()) {
				gui_->addMessageChat(eColorLog::kRed,
									 ErrorServerAlreadyUseOnThisPort);
			} else {
				gui_->addMessageChat(eColorLog::kRed, ex.what());
			}
		} catch (std::exception const &e) {
			gui_->addMessageChat(e.what());
		}
	}


}

void Univers::createBobby() {
	if (!isServer()) {
		gui_->addMessageChat(eColorLog::kOrange, WarningServerCreateIA);
		return;
	}
	if (snakeServer_->isFull()) {
		gui_->addMessageChat(eColorLog::kOrange, WarningServerFull);
		return;
	}
	std::unique_ptr<Bobby> bobby = std::make_unique<Bobby>(*this);
	try {
		bobby->getClientTCP_()->connect(DEFAULT_DNS, std::to_string(
				snakeServer_->getPort_()));
	} catch (const std::exception &e) {
		gui_->addMessageChat(e.what());
	}
	vecBobby.push_back(std::move(bobby));
	gui_->addMessageChat(eColorLog::kGreen, SuccessIAIsCreate);
}

void Univers::createClient() {
	log_info("%s", __PRETTY_FUNCTION__);

	if (!snakeClient_) {
		snakeClient_ = SnakeClient::create(*this, false);
		gui_->addMessageChat(eColorLog::kGreen, SuccessClientIsCreate);
	} else
		gui_->addMessageChat(eColorLog::kOrange, WarningClientExist);
}


void Univers::createGui() {
	gui_ = std::make_unique<Gui>(*this);
}


void Univers::disconnectClient() {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (ptr) {
		ptr->disconnect();
		gui_->addMessageChat(eColorLog::kGreen, SuccessClientIsDelete); //TODO const
	} else
		gui_->addMessageChat(eColorLog::kOrange, WarningClientNotExist);
}

void Univers::closeServer() {
	boost::shared_ptr<SnakeServer> ptr(snakeServer_);

	if (ptr) {
		ptr->closeAcceptorServer();
	} else {
		//TODO const
	}
}

void Univers::deleteBobby(int16_t id) {
	if (!isServer()) {
		gui_->addMessageChat(eColorLog::kOrange, WarningServerRemoveIA);
		return;
	}
	vecBobby.erase(std::remove_if(vecBobby.begin(), vecBobby.end(),
			[id](std::unique_ptr<Bobby> &bob) {

		if (bob->getId() == id) {
			bob->getClientTCP_()->disconnect();
		}
		return bob->getId() == id;

	}), vecBobby.end());
}

void Univers::deleteClient() {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (ptr) {
		ptr->disconnect();
		gui_->addMessageChat(eColorLog::kGreen, SuccessClientIsDelete);
		snakeClient_ = nullptr;
	} else
		gui_->addMessageChat(eColorLog::kOrange, WarningClientNotExist);
}

void Univers::deleteServer() {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (snakeServer_) {
		snakeServer_ = nullptr;
		gui_->addMessageChat(eColorLog::kGreen, SuccessServerIsDelete);
		if (ptr && ptr->isOpen())
			deleteClient();
	} else {
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
	getSoundManager().playMusic((pathSound / "zelda.ogg").generic_string());
}

void Univers::loadSound(eSound sound) {
	if (getSoundManager().hasLibraryLoaded())
		throw std::runtime_error("Trying to loadExternalSoundLibrary but it is already loaded");
	getSoundManager().loadExternalSoundLibrary(sound);
	getSoundManager().constructExternalLibrary();
	loadSoundData_();
}
void Univers::unloadSound() {
	if (!getSoundManager().hasLibraryLoaded())
		throw std::runtime_error("Trying to unloadExternalSoundLibrary but it is already unload");
	getSoundManager().unloadExternalSoundLibrary();
}

/** Getter && Setter **/

uint32_t Univers::getMicroSecDeltaTime() const {
	return microSecDeltaTime;
}

void Univers::setMicroSecDeltaTime(uint32_t microSecDeltaTime) {
	Univers::microSecDeltaTime = microSecDeltaTime < GameManager::Impossible ? GameManager::Impossible : microSecDeltaTime;
}

void Univers::setBaseMicroSecDeltaTime(uint32_t BaseMicroSecDeltaTime) {
	Univers::BaseMicroSecDeltaTime = BaseMicroSecDeltaTime;
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

const SnakeArrayContainer &Univers::getSnakeArray_() const {
	SnakeClient::boost_shared_ptr ptr(getSnakeClient().lock());

	if (isServer())
		return snakeServer_->getSnakeArray_();
	if (ptr)
		return ptr->getSnakeArray_();
	return snakeArrayContainer;
}

boost::weak_ptr<ISnakeNetwork> Univers::getSnakeNetwork() const {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (ptr && ptr->isOpen())
		return ptr->shared_from_this();
	else if (vecBobby.size() != 0)
		return vecBobby.front()->getClientTCP_()->shared_from_this();
	else if (isServer())
		return snakeServer_->shared_from_this();
	return boost::weak_ptr<SnakeClient>();
}

boost::weak_ptr<SnakeClient> Univers::getSnakeClient() const {
	SnakeClient::boost_shared_ptr ptr(snakeClient_);

	if (ptr && ptr->isOpen())
		return ptr->shared_from_this();
	else if (vecBobby.size() != 0)
		return vecBobby.front()->getClientTCP_()->shared_from_this();
	return boost::weak_ptr<SnakeClient>();
}

const unsigned int &Univers::getMapSize() const {
	return mapSize_;
}

void Univers::setMapSize(unsigned int mapSize) {
	mapSize_ = mapSize;
}

void Univers::setBorderless(bool borderless) {
	Univers::borderless = borderless;
}

bool Univers::isServer() const {
	return snakeServer_ != nullptr;
}

const bool &Univers::isBorderless() const {
	return borderless;
}

bool Univers::isOnlyIA() const {
	return (snakeClient_ == nullptr || !snakeClient_->isOpen()) && vecBobby.size() != 0;
}


bool Univers::isOpenGame_() const {
	return openGame_;
}

void Univers::setOpenGame_(bool openGame) {
	Univers::openGame_ = openGame;
}

void Univers::sendHostname() {
	if (gui_) {
		char hostname[64];
		gethostname(hostname, 64);
		gui_->addMessageChat(std::string("[INFO] ") + hostname);
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
		!displayManager->getDisplay()->exit();
}

bool Univers::isSwitchLib() const {
	return switchLib;
}

void Univers::updateDisplayUI() {
	assert(displayManager != nullptr);
	assert(displayManager->getDisplay() != nullptr);

	displayManager->getDisplay()->update();
	displayManager->getDisplay()->drawGrid(*grid_);
	displayManager->getDisplay()->render();
}

ExternalLibrarySoundManager &Univers::getSoundManager() {
	return *soundManager;
}

SnakeServer &Univers::getSnakeServer() const {
	return *snakeServer_;
}

Univers::BobbyContainer &Univers::getBobbys() {
	return vecBobby;
}

GameManager &Univers::getGameManager() {
	return *gameManager;
}

void Univers::switchBorderless() {

	setBorderless(!isBorderless());

	if (isBorderless())
		gui_->addMessageChat(eColorLog::kGreen, SuccessBorderlessSet);
	else
		gui_->addMessageChat(eColorLog::kGreen, SuccessBorderlessUnset);

	boost::shared_ptr<ISnakeNetwork> ptr_network(getSnakeNetwork().lock());

	if (!ptr_network || !ptr_network->isOpen()) {
		return;
	}
	ptr_network->notifyBorderless();

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
	SnakeClient::boost_shared_ptr ptr(getSnakeClient());

	if (!isServer()) {
		gui_->addMessageChat(eColorLog::kOrange,
							 WarningUserIsNotTheServer);
		return;
	}
	if (!ptr) {
		gui_->addMessageChat(eColorLog::kOrange, WarningRequiredAtLeastOneClient);
		return;
	}
	if (!snakeServer_->isReady()) {
		gui_->addMessageChat(eColorLog::kOrange, WarningAllClientIsNotReady);
		return;
	}
	snakeServer_->sendOpenGameToClient();
}

void Univers::updateSizeMap() {
	boost::shared_ptr<ISnakeNetwork> ptr_network(getSnakeNetwork().lock());
	if (!ptr_network) {
		gui_->addMessageChat(eColorLog::kOrange, WarningClientNotExist);//Todo message
		return;
	}
	if (!ptr_network->isOpen()) {
		gui_->addMessageChat(eColorLog::kOrange, WarningClientIsNotConnected); //Todo message
		return;
	}
	ptr_network->notifyMapSize();
}