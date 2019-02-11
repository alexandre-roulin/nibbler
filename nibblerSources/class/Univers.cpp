#include "Univers.hpp"
#include <gui/Gui.hpp>
#include <systems/MotionSystem.hpp>
#include <systems/JoystickSystem.hpp>
#include <systems/FollowSystem.hpp>
#include <systems/FoodCreationSystem.hpp>
#include <systems/CollisionSystem.hpp>
#include <systems/SpriteSystem.hpp>
#include <systems/RenderSystem.hpp>
#include <systems/FoodEatSystem.hpp>
#include <events/FoodEat.hpp>
#include <KINU/World.hpp>
#include <events/StartEvent.hpp>
#include <events/FoodCreation.hpp>
#include <events/JoystickEvent.hpp>
#include <dlfcn.h>
#include <network/SnakeServer.hpp>
#include <network/SnakeClient.hpp>

/** Const Variable **/
const std::string Univers::SuccessServerIsCreate = "Server is online.";
const std::string Univers::SuccessIAIsCreate = "IA is online.";
const std::string Univers::SuccessClientIsConnected = "Client is connected.";
const std::string Univers::SuccessClientIsCreate = "Client is online";
const std::string Univers::SuccessClientIsDelete = "Client is delete.";
const std::string Univers::SuccessServerIsDelete = "Server is delete.";

const std::string Univers::WarningClientExist = "Client is already in place.";
const std::string Univers::WarningServerCreateIA = "Only the server owner can create IA.";
const std::string Univers::WarningServerRemoveIA = "Only the server owner can remove IA.";
const std::string Univers::WarningServerFull = "Server have reach the maximum player.";
const std::string Univers::WarningServerExist = "Server is left online.";
const std::string Univers::WarningClientNotExist = "There is no client online.";
const std::string Univers::WarningServerNotExist = "There is no server online.";
const std::string Univers::WarningClientIsAlreadyConnected = "Client is already connected.";
const std::string Univers::WarningClientIsNotConnected = "Client is not connected.";
const std::string Univers::WarningClientConnectError = "Connection failed.";
const std::string Univers::WarningUserIsNotTheServer = "User is not the server.";
const std::string Univers::WarningRequiredAtLeastOneClient = "You need to have at least one client to start game.";
const std::string Univers::ErrorClientConnectionRefused = "Connection refused.";
const std::string Univers::ErrorServerAlreadyUseOnThisPort = "Server already in use on this port.";

Univers::Univers()
		: pathRoot_(NIBBLER_ROOT_PROJECT_PATH),
		  flag_(0),
		  exit_(false),
		  switchLib(false),
		  timer_loop(boost::asio::deadline_timer(io_loop)),
		  timer_start(boost::asio::deadline_timer(io_start)),
		  world_(nullptr),
		  snakeServer_(nullptr),
		  snakeClient_(nullptr),
		  gui_(nullptr),
		  grid_(nullptr),
		  mapSize_(MAP_DEFAULT),
		  gameSpeed(1),
		  dlHandleDisplay(nullptr),
		  dlHandleSound(nullptr),
		  display(nullptr),
		  sound(nullptr),
		  borderless(false),
		  openGame_(false),
		  kDisplay(eDisplay::kDisplaySfmlLibrary) {
}

/** External Library Management **/


bool Univers::load_external_sound_library(std::string const &library_path) {
	if (sound != nullptr && dlHandleSound != nullptr) {
		if (deleteSound) {
			deleteSound(sound);
			deleteSound = nullptr;
			newSound = nullptr;
		}
		dlclose(dlHandleSound);
	}

	if (!(dlHandleSound = dlopen(library_path.c_str(), RTLD_LAZY | RTLD_LOCAL)))
		return (dlError("dlopen"));

	if (!(newSound = reinterpret_cast<ISound *(*)(
			const char *) >(dlsym(dlHandleSound, "newSound"))))
		return (dlError("dlsym"));

	if (!(deleteSound = reinterpret_cast<void (*)(
			ISound *)>(dlsym(dlHandleSound, "deleteSound"))))
		return (dlError("dlsym"));
	return (sound = newSound(library_path.c_str())) != nullptr;
}

bool Univers::load_extern_lib_sound(Univers::eSound eLib) {
	flag_.set(eFlag::kSound);
	switch (eLib) {
		case kSoundSdlLibrary : {
			log_success("Univers::load_extern_lib_sound.kSoundSdlLibrary");
		}
		case kSoundSfmlLibrary : {
			log_success("Univers::load_extern_lib_sound.kSoundSfmlLibrary");
			return load_external_sound_library(boost::filesystem::path(PATH_SOUND_LIBRARY_SFML).generic_string());
		}
	}
	flag_.reset(eFlag::kSound);
	return false;
}

void Univers::unload_external_sound_library() {
	log_error("Univers::unload_external_sound_library");
	if (sound != nullptr && dlHandleSound != nullptr) {
		if (deleteSound) {
			deleteSound(sound);
			deleteSound = nullptr;
			newSound = nullptr;
			sound = nullptr;
		}
		dlclose(dlHandleSound);
		dlHandleSound = nullptr;
		flag_.reset(eFlag::kSound);
	}
	log_error("Univers::unload_external_sound_library.unlock()");
}

bool Univers::load_external_display_library(std::string const &title,
											std::string const &libPath) {

	if (!(dlHandleDisplay = dlopen(libPath.c_str(), RTLD_LAZY | RTLD_LOCAL)))
		return (dlError("dlopen"));
	if (!(newDisplay = reinterpret_cast<IDisplay *(*)(
			int, int, const char *
	)>(dlsym(dlHandleDisplay, "newDisplay"))))
		return (dlError("dlsym"));
	if (!(deleteDisplay = reinterpret_cast<void (*)(
			IDisplay *
	)>(dlsym(dlHandleDisplay, "deleteDisplay"))))
		return (dlError("dlsym"));
	if (!(display = newDisplay(mapSize_, mapSize_, title.c_str())))
		return (false);

	log_success("load_external_display_library up !");
	return (true);
}

bool Univers::load_extern_lib_display(Univers::eDisplay eLib) {
	switch (eLib) {
		case kDisplaySfmlLibrary : {
			log_success("Univers::load_extern_lib_display.kDisplaySfmlLibrary");
			return load_external_display_library(
					"Nibbler - SFML",
					boost::filesystem::path(PATH_DISPLAY_LIBRARY_SFML).generic_string()
					);
		}
		case kDisplaySdlLibrary : {
			log_success("Univers::load_extern_lib_display.kDisplaySdlLibrary");
			return load_external_display_library(
					"Nibbler - SDL",
					boost::filesystem::path(PATH_DISPLAY_LIBRARY_SDL).generic_string()
			);		}
		case kDisplayGlfwLibrary : {
			log_success("Univers::load_extern_lib_display.kDisplayGlfwLibrary");
			return load_external_display_library(
					"Nibbler - GLFW",
					boost::filesystem::path(PATH_DISPLAY_LIBRARY_GLFW).generic_string()
			);		}
	}
	return false;
}

void Univers::unload_external_display_library() {
	log_error("Univers::unload_external_display_library");
	if (display != nullptr && dlHandleDisplay != nullptr) {
		if (deleteDisplay) {
			deleteDisplay(display);
			deleteDisplay = nullptr;
			newDisplay = nullptr;
			display = nullptr;
		}
		dlclose(dlHandleDisplay);
		dlHandleDisplay = nullptr;
	}
	log_error("Univers::unload_external_display_library.unlock()");
}

/** Game Management **/


void Univers::defaultAssignmentLibrary() {
	if (!display) return;

	MutantGrid<eSprite> grid(mapSize_);
	grid.fill(eSprite::kGround);
	display->setBackground(grid);
	display->registerCallbackAction(std::bind(&Univers::callbackAction, this, std::placeholders::_1));
	display->update();
	display->render();
}

void Univers::new_game() {
	log_info("%s", __PRETTY_FUNCTION__);
	io_loop.restart();
	io_start.restart();
	log_info("Hello i'am a %s", isServer() ? "Server" : "Client");
	world_ = std::make_unique<KINU::World>();
	world_->getEventsManager().destroy<FoodCreation>();
	grid_ = std::make_shared<MutantGrid<eSprite>>(mapSize_);
	grid_->fill(eSprite::kNone);
	if (!display) load_extern_lib_display(kDisplay);
	defaultAssignmentLibrary();
	if (isServer()) {
		snakeServer_->startGame();
		for (std::unique_ptr<Bobby> &bobby : vecBobby) {
			bobby->buildIA();
			bobby->sendDirection();
		}

	}

	world_->getSystemsManager().addSystem<CollisionSystem>(*this);
	world_->getSystemsManager().addSystem<FollowSystem>();
	world_->getSystemsManager().addSystem<JoystickSystem>(*this);
	world_->getSystemsManager().addSystem<MotionSystem>(*this);
	world_->getSystemsManager().addSystem<SpriteSystem>(*this);
	world_->getSystemsManager().addSystem<RenderSystem>(*this);
	world_->getSystemsManager().addSystem<FoodCreationSystem>();
	world_->getSystemsManager().addSystem<FoodEatSystem>();

	manage_start();

	world_->update();
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
	world_->getSystemsManager().getSystem<RenderSystem>().update();

	timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
	thread = boost::thread(
			boost::bind(&boost::asio::io_service::run, &io_loop));
	thread.detach();

	loop();
}

void Univers::manage_input() {
	log_info("%s %d",__PRETTY_FUNCTION__,  snakeClient_ != nullptr);
	if (snakeClient_) {
		if (snakeClient_->getSnake().isAlive)
			snakeClient_->sendDataToServer(InputInfo(snakeClient_->getId_(),
					(display ? display->getDirection() : eDirection::kNorth)),
							eHeader::kInput);
	}

}

void Univers::manage_start() {
	log_info("%s", __PRETTY_FUNCTION__);
	StartInfo startInfo;
	std::vector<StartEvent> startEvent;
	for (; startEvent.empty();) {

		getSnakeClient()->lock();
		startEvent = world_->getEventsManager().getEvents<StartEvent>();
		getSnakeClient()->unlock();
	}
	auto ptime = startEvent.front().start_time;
	timer_start.expires_at(ptime);
	io_start.run();
	timer_start.wait();
	timer_loop.expires_at(ptime + boost::posix_time::milliseconds(gameSpeed));
	if (isServer()) {
		for (auto &bobby : vecBobby) {
			bobby->sendDirection();
		}
	}
}


void Univers::loop() {
	log_info("%s", __PRETTY_FUNCTION__);

	std::cout << "AllSnakeIsDead : " << getSnakeClient()->allSnakeIsDead() << std::endl;
	while (openGame_ && (display == nullptr || !display->exit()) &&
		   !getSnakeClient()->allSnakeIsDead()) {
		if (switchLib)
			manageSwitchLibrary();
		display->update();
		display->drawGrid(*grid_);
		display->render();
	}
	unload_external_display_library();
	finish_game();
}

void Univers::loop_world() {
	log_warn("World is not %s !!!", world_ ? "UP" : "DOWN");
	if (!openGame_ || !getSnakeClient())
		return;
	manage_input();

	for (; nextFrame.empty() && openGame_ && world_ && getSnakeClient();) {
		getSnakeClient()->lock();
//		 std::cout << "Stuck nextFrame" << std::endl;
		nextFrame = world_->getEventsManager().getEvents<NextFrame>();
		getSnakeClient()->unlock();
	}
	if (!openGame_ || !getSnakeClient())
		return;
	nextFrame.clear();
	world_->getEventsManager().destroy<NextFrame>();

	getSnakeClient()->deliverEvents();

	world_->update();

//	log_info("Univers::FollowSystem");
	world_->getSystemsManager().getSystem<FollowSystem>().update();
//	log_info("Univers::JoystickSystem");
	world_->getSystemsManager().getSystem<JoystickSystem>().update();
//	log_info("Univers::JoystickEvent");
	world_->getEventsManager().destroy<JoystickEvent>();
//	log_info("Univers::MotionSystem");
	world_->getSystemsManager().getSystem<MotionSystem>().update();
//	log_info("Univers::CollisionSystem");
	world_->getSystemsManager().getSystem<CollisionSystem>().update();
//	log_info("Univers::FoodCreationSystem");
	world_->getSystemsManager().getSystem<FoodCreationSystem>().update();
	world_->getEventsManager().destroy<FoodCreation>();
//	log_info("Univers::SpriteSystem");
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
//	log_info("Univers::RenderSystem");
	world_->getSystemsManager().getSystem<RenderSystem>().update();
//	log_info("Univers::FoodEatSystem");

//	log_info("Univers::vecBobby");
	Bobby::clearPriority();
	if (isServer()) {
		for (auto &bobby : vecBobby) {
			if (world_->getEntitiesManager().hasEntityByTagId(
					bobby->getId() + eTag::kHeadTag)) {
				boost::thread
						t2(
						boost::bind(&Bobby::calculateDirection, bobby.get()));
				t2.detach();
			}

		}
	}


	world_->getSystemsManager().getSystem<FoodEatSystem>().update();
	world_->getEventsManager().destroy<FoodEat>();

	if (!getSnakeClient()->allSnakeIsDead()) {

		timer_loop.expires_at(
				timer_loop.expires_at() +
				boost::posix_time::milliseconds(gameSpeed));
		timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
	}
//	world_->grid.print();
}

void Univers::manageSwitchLibrary() {
	log_success("sw %d", !getSnakeClient()->isSwitchingLibrary());
	if (!getSnakeClient()->isSwitchingLibrary()) {
		int16_t id = getSnakeClient()->getId_();
		getSnakeClient()->sendDataToServer(id, eHeader::kForcePause);
		kDisplay = (kDisplay == kDisplayGlfwLibrary) ? kDisplaySfmlLibrary : static_cast<eDisplay>(kDisplay + 1);
		unload_external_display_library();
		std::cout << load_extern_lib_display(kDisplay) << std::endl;
		defaultAssignmentLibrary();
		getSnakeClient()->sendDataToServer(id, eHeader::kForcePause);
	}
	switchLib = false;
}

void Univers::refreshTimerLoopWorld() {
	timer_loop.expires_at(boost::posix_time::microsec_clock::universal_time());
}

void Univers::callbackAction(eAction action) {
//	if (getSnakeClient() == nullptr) return;
	switch (action) {
		case eAction::kHostname :
			sendHostname();
			break;
		case eAction::kPause :
			log_success(" eAction::kPause");
			getSnakeClient()->sendDataToServer(action, eHeader::kPause);
			break;
		case eAction::kSwitchDisplayLibrary:
			log_success("eAction::kSwitchDisplayLibrary");
			switchLib = true;
			break;
		case eAction::kCreateClient :
			create_client();
			break;
		case eAction::kDeleteClient :
			deleteClient();
			break;
		case eAction::kCreateServer :
			create_server();
			break;
		case eAction::kDeleteServer :
			deleteServer();
			break;
		case eAction::kCreateIA :
			create_ia();
			break;
		case eAction::kDeleteIA :
			break;
		case eAction::kConnect :
			connect();
			break;
		case eAction::kBorderless :
			if (!getSnakeClient()) {
				gui_->addMessageChat(WarningClientNotExist);
				break;
			}
			if (!getSnakeClient()->isConnect()) {
				gui_->addMessageChat(WarningClientIsNotConnected);
				break;
			}
			getSnakeClient()->changeIsBorderless(!isBorderless());
			break;
		case eAction::kSwitchReady :
			if (!getSnakeClient()) {
				gui_->addMessageChat(WarningClientNotExist);
				break;
			}
			if (!getSnakeClient()->isConnect()) {
				gui_->addMessageChat(WarningClientIsNotConnected);
				break;
			}
			getSnakeClient()->changeStateReady(!getSnakeClient()->isReady());
			break;
		case eAction::kStartGame :
			if (!isServer()) {
				gui_->addMessageChat(WarningUserIsNotTheServer);
				break;
			}
			if (!getSnakeClient()) {
				gui_->addMessageChat(WarningRequiredAtLeastOneClient);
				break;
			}
			snakeServer_->sendOpenGameToClient();
			break;
	}
}

/** Create and delete **/


void Univers::create_client() {
	if (!snakeClient_) {
		snakeClient_ = SnakeClient::create(*this, false);
		gui_->addMessageChat(SuccessClientIsCreate);
	}
	else
		gui_->addMessageChat(WarningClientExist);
}

void Univers::create_server(unsigned int port) {
	if (snakeServer_)
		gui_->addMessageChat(WarningServerExist);
	else {
		try {
			snakeServer_ = SnakeServer::create(*this, port);
			gui_->addMessageChat(SuccessServerIsCreate);
		} catch (const boost::system::system_error& ex) {
			if (boost::system::errc::address_in_use == ex.code()) {
				gui_->addMessageChat(ErrorServerAlreadyUseOnThisPort);
			} else {
				gui_->addMessageChat(ex.what());
			}
		} catch (std::exception const &e) {
			gui_->addMessageChat(e.what());
		}
	}


}

void Univers::connect(const std::string &dns, const std::string &port) {
	if (!snakeClient_) {
		gui_->addMessageChat(WarningClientNotExist);
		return;
	}
	if (snakeClient_->isConnect()) {
		gui_->addMessageChat(WarningClientIsAlreadyConnected);
		return;
	}
	try {
		snakeClient_->connect(dns, port);
		if (snakeClient_->isConnect())
			gui_->addMessageChat(SuccessClientIsConnected);
		else
			gui_->addMessageChat(SuccessClientIsConnected);
	} catch (const boost::system::system_error& ex) {
		if (ex.code() == boost::system::errc::connection_refused) {
			gui_->addMessageChat(ErrorClientConnectionRefused);
		} else {
			gui_->addMessageChat(ex.what());
		}
	} catch (const std::exception &e) {
		gui_->addMessageChat(e.what());
	}
}

void Univers::create_ia() {
	if (!isServer()) {
		gui_->addMessageChat(WarningServerCreateIA);
		return;
	}
	if (snakeServer_->isFull()) {
		gui_->addMessageChat(WarningServerFull);
		return;
	}
	std::unique_ptr<Bobby> bobby = std::make_unique<Bobby>(*this);
	bobby->getClientTCP_()->connect(LOCALHOST, std::to_string(snakeServer_->getPort_()));
	vecBobby.push_back(std::move(bobby));
	gui_->addMessageChat(SuccessIAIsCreate);
}

void Univers::delete_ia(int16_t id) {
	//if (!isServer()) {
	//	gui_->addMessageChat(WarningServerRemoveIA);
	//	return;
	//}
	vecBobby.erase(std::remove_if(vecBobby.begin(), vecBobby.end(),
							 [this, id](const std::unique_ptr<Bobby> &bob){
		if (bob->getId() == id) {
			getSnakeArray_()[bob->getId()].isValid = false;
			bob->getClientTCP_()->disconnect();
		}
		return bob->getId() == id;
	}), vecBobby.end());
}

void Univers::delete_ia() {
	//if (!isServer()) {
	//	gui_->addMessageChat(WarningServerRemoveIA);
	//	return;
	//}
	for (auto &bobby : vecBobby) {
		getSnakeArray_()[bobby->getId()].isValid = false;
		bobby->getClientTCP_()->disconnect();
	}
	vecBobby.clear();
}

void Univers::deleteServer() {
	if (snakeServer_) {
		snakeServer_ = nullptr;
		vecBobby.clear();
		gui_->addMessageChat(SuccessServerIsDelete);
		if (snakeClient_ && snakeClient_->isConnect())
			deleteClient();
	} else {
		gui_->addMessageChat(WarningServerNotExist);
	}
}

void Univers::deleteClient() {
	if (snakeClient_) {
		log_fatal("use count %d", snakeClient_.use_count());
		snakeClient_->disconnect();

		gui_->addMessageChat(SuccessClientIsDelete);
	}
	else
		gui_->addMessageChat(WarningClientNotExist);
}

void Univers::finish_game() {
	cleanAll();
}

void Univers::createGui() {
	gui_ = std::make_unique<Gui>(*this);
}

void Univers::deleteGui() {
	gui_.reset(nullptr);
}

/** Getter && Setter **/

bool Univers::isExit() const {
	return exit_;
}

void Univers::setExit(bool b) {
	exit_ = b;
}

std::unique_ptr<Gui> &Univers::getGui_() {
	return gui_;
}

MutantGrid<eSprite> &Univers::getGrid_() {
	return *grid_;
}

std::array<Snake, SNAKE_MAX> Univers::getSnakeArray_() const {

	if (isServer())
		return snakeServer_->getSnakeArray_();
	if (getSnakeClient())
		return getSnakeClient()->getSnakeArray_();

	return std::array<Snake, SNAKE_MAX>();
}


KINU::World &Univers::getWorld_() const {
	return *world_;
}

boost::shared_ptr<SnakeClient> Univers::getSnakeClient() const {
	if (snakeClient_)
		return snakeClient_->shared_from_this();
	else if (vecBobby.size() != 0)
		return vecBobby.front()->getClientTCP_()->shared_from_this();
	return nullptr;
}

ISound &Univers::getSound() const {
	return *sound;
}

unsigned int Univers::getMapSize() const {
	return mapSize_;
}

void Univers::setMapSize(unsigned int mapSize) {
	mapSize_ = mapSize;
}

void Univers::setBorderless(bool borderless) {
	Univers::borderless = borderless;
}

/** State **/

bool Univers::isServer() const {
	return snakeServer_ != nullptr;
}

bool Univers::isBorderless() const {
	return borderless;
}

bool Univers::isOnlyIA() const {
	return snakeClient_ == nullptr && vecBobby.size() != 0;
}

/** Sound **/

void Univers::addNoise(std::string const &path) {
	if (sound && flag_.test(eFlag::kSound))
		sound->addNoise(path);
}
void Univers::playNoise(eNoise e) const {
	if (sound && flag_.test(eFlag::kSound))
		sound->playNoise(static_cast<int>(e));
}
void Univers::playMusic(std::string const &path) const {
	if (sound && flag_.test(eFlag::kSound)) {
		sound->setMusic(path.c_str());
		sound->playMusic();
	}
}

/** Error **/

bool Univers::dlError(char const *from) {
	std::cerr << "Error " << from << " : " << dlerror() << std::endl;
	dlclose(dlHandleDisplay);
	return (false);
}

bool Univers::isIASnake(uint16_t client_id) const {

	for (auto &bobby : vecBobby) {
		if (bobby->getId() == client_id)
			return true;
	}
	return false;
}

Univers::~Univers() {
	log_warn("~Univers");
	cleanAll();
	snakeClient_ = nullptr;
	snakeServer_ = nullptr;
	unload_external_display_library();
	unload_external_sound_library();
	log_warn("~Univers.end()");
}

bool Univers::isOpenGame_() const {
	return openGame_;
}

void Univers::setOpenGame_(bool openGame) {
	Univers::openGame_ = openGame;
}

void Univers::cleanAll() {
	openGame_ = false;
	timer_loop.wait();
	timer_start.wait();
	thread.join();
	io_loop.reset();
	io_start.reset();
	timer_loop.cancel();
	timer_start.cancel();
	switchLib = false;
	nextFrame.clear();
	world_ = nullptr;
	if (getSnakeClient() && !getSnakeClient()->isConnect()) {
		deleteServer();
		deleteClient();
		borderless = false;
		mapSize_ = MAP_DEFAULT;
	}
	gui_ = nullptr;
	grid_ = nullptr;
}

void Univers::sendHostname() {
	if (gui_) {
		char hostname[64];
		gethostname(hostname, 64);
		gui_->addMessageChat(std::string("[INFO] ") + hostname);
	}

}


