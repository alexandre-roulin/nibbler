#include "Univers.hpp"
#include <gui/Core.hpp>
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
const std::string Univers::WarningServerFull = "Server have reach the maximum player.";
const std::string Univers::WarningServerExist = "Server is left online.";
const std::string Univers::WarningClientNotExist = "There is no client online.";
const std::string Univers::WarningServerNotExist = "There is no server online.";
const std::string Univers::WarningClientIsAlreadyConnected = "Client is already connected.";
const std::string Univers::WarningClientIsNotConnected = "Client is not connected.";
const std::string Univers::WarningUserIsNotTheServer = "User is not the server.";
const std::string Univers::WarningRequiredAtLeastOneClient = "You need to have at least one client to start game.";

Univers::Univers()
		: pathRoot_(NIBBLER_ROOT_PROJECT_PATH),
		  flag_(0),
		  switchLib(false),
		  timer_loop(boost::asio::deadline_timer(io_loop)),
		  timer_start(boost::asio::deadline_timer(io_start)),
		  world_(nullptr),
		  serverTCP_(nullptr),
		  clientTCP_(nullptr),
		  core_(nullptr),
		  grid_(nullptr),
		  mapSize_(MAP_DEFAULT),
		  gameSpeed(100),
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
	flag_.set(eFlag::SOUND);
	switch (eLib) {
		case kSoundSdlLibrary : {
			log_success("Univers::load_extern_lib_sound.kSoundSdlLibrary");
		}
		case kSoundSfmlLibrary : {
			log_success("Univers::load_extern_lib_sound.kSoundSfmlLibrary");
			return load_external_sound_library(boost::filesystem::path(PATH_SOUND_LIBRARY_SFML).generic_string());
		}
	}
	flag_.reset(eFlag::SOUND);
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
		flag_.reset(eFlag::SOUND);
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
	grid.fill(eSprite::GROUND);
	display->setBackground(grid);
	display->registerCallbackAction(std::bind(&Univers::callbackAction, this, std::placeholders::_1));
	display->update(0.17f);
	display->render(1.f, 1.f);
}

void Univers::new_game() {
	log_info("%s", __PRETTY_FUNCTION__);
	io_loop.restart();
	io_start.restart();
	log_info("Hello i'am a %s", isServer() ? "Server" : "Client");
	world_ = std::make_unique<KINU::World>();
	world_->getEventsManager().destroy<FoodCreation>();
	grid_ = std::make_shared<MutantGrid<eSprite>>(mapSize_);
	grid_->fill(eSprite::NONE);
	if (!display) load_extern_lib_display(kDisplay);
	defaultAssignmentLibrary();
	if (isServer()) {
		serverTCP_->startGame();
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
	log_info("%s %d",__PRETTY_FUNCTION__,  clientTCP_ != nullptr);
	if (clientTCP_) {
		if (clientTCP_->getSnake().isAlive)
			clientTCP_->sendDataToServer(InputInfo(clientTCP_->getId_(),
					(display ? display->getDirection() : eDirection::kNorth)),
							eHeaderK::kInput);
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

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds current(0);
	std::cout << "AllSnakeIsDead : " << getSnakeClient()->allSnakeIsDead() << std::endl;
	while (openGame_ && (display == nullptr || !display->exit()) &&
		   !getSnakeClient()->allSnakeIsDead()) {
		if (switchLib)
			manageSwitchLibrary();
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		t1 = t2;
		current += time_span;

		if (current > std::chrono::milliseconds(gameSpeed))
			current = std::chrono::milliseconds(gameSpeed);

		//std::cout << "It took me " << current.count() << "," << gameSpeed << " seconds." << std::endl;
		display->update(0.017f);
		display->drawGrid(*grid_);
		display->render(0.017f, 1.f);
//			display->update(0.2f);
//			display->drawGrid(world_->grid);
//			display->render(current.count(), gameSpeed);
//			display->render(gameSpeed, gameSpeed);
		if (current >= std::chrono::milliseconds(gameSpeed)) {
			current = std::chrono::milliseconds(0);
		}
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
					bobby->getId() + eTag::HEAD_TAG)) {
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
		getSnakeClient()->sendDataToServer(id, eHeaderK::kForcePause);
		kDisplay = (kDisplay == kDisplayGlfwLibrary) ? kDisplaySfmlLibrary : static_cast<eDisplay>(kDisplay + 1);
		unload_external_display_library();
		std::cout << load_extern_lib_display(kDisplay) << std::endl;
		defaultAssignmentLibrary();
		getSnakeClient()->sendDataToServer(id, eHeaderK::kForcePause);
	}
	switchLib = false;
}

void Univers::refreshTimerLoopWorld() {
	timer_loop.expires_at(boost::posix_time::microsec_clock::universal_time());
}

void Univers::callbackAction(eAction action) {
//	if (getSnakeClient() == nullptr) return;
	switch (action) {
		case eAction::kPause :
			log_success(" eAction::kPause");
			getSnakeClient()->sendDataToServer(action, eHeaderK::kPause);
			break;
		case eAction::kSwitchDisplayLibrary:
			log_success("eAction::kSwitchDisplayLibrary");
			switchLib = true;
			break;
		case eAction::kCreateClient :
			create_client();
			break;
		case eAction::kDeleteClient :
			delete_client();
			break;
		case eAction::kCreateServer :
			create_server();
			break;
		case eAction::kDeleteServer :
			delete_server();
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
			if (getSnakeClient())
				getSnakeClient()->changeIsBorderless(!isBorderless());
			else
				core_->addMessageChat(WarningClientNotExist);
			break;
		case eAction::kReady :
			if (!getSnakeClient()) {
				core_->addMessageChat(WarningClientNotExist);
				break;
			}
			if (!getSnakeClient()->isConnect()) {
				core_->addMessageChat(WarningClientIsNotConnected);
				break;
			}
			getSnakeClient()->changeStateReady(!getSnakeClient()->isReady());
			break;
		case eAction::kStartGame :
			if (!isServer()) {
				core_->addMessageChat(WarningUserIsNotTheServer);
				break;
			}
			if (!getSnakeClient()) {
				core_->addMessageChat(WarningRequiredAtLeastOneClient);
				break;
			}
			serverTCP_->sendOpenGameToClient();
			break;
	}
}

/** Create and delete **/


void Univers::create_client() {
	if (!clientTCP_) {
		clientTCP_ = SnakeClient::create(*this, false);
		core_->addMessageChat(SuccessClientIsCreate);
	}
	else
		core_->addMessageChat(WarningClientExist);
}

void Univers::create_ui() {
	core_ = std::make_unique<Core>(*this);
}

void Univers::create_server(unsigned int port) {
	if (serverTCP_)
		core_->addMessageChat(WarningServerExist);
	else {
		try {
			serverTCP_ = SnakeServer::create(*this, port);
			core_->addMessageChat(SuccessServerIsCreate);
		} catch (std::exception const &e) {
			core_->addMessageChat(e.what());
		}
	}


}

void Univers::connect(const std::string &dns, const std::string &port) {
	if (!clientTCP_) {
		core_->addMessageChat(WarningClientNotExist);
		return;
	}
	log_fatal("connect %d", getSnakeClient()->isConnect());
	if (clientTCP_->isConnect()) {
		core_->addMessageChat(WarningClientIsAlreadyConnected);
		return;
	}
	try {
		clientTCP_->connect(dns, port);
		core_->addMessageChat(SuccessClientIsConnected);
	} catch (std::exception const &e) {
		core_->addMessageChat(e.what());
	}
}

void Univers::create_ia() {
	if (!isServer()) {
		core_->addMessageChat(WarningServerCreateIA);
		return;
	}
	if (serverTCP_->isFull()) {
		core_->addMessageChat(WarningServerFull);
		return;
	}
	std::unique_ptr<Bobby> bobby = std::make_unique<Bobby>(*this);
	bobby->getClientTCP_()->connect("localhost", std::to_string(serverTCP_->getPort_()));
	vecBobby.push_back(std::move(bobby));
	core_->addMessageChat(SuccessIAIsCreate);
}

void Univers::delete_ia() {
	for (auto &bobby : vecBobby) {
		getSnakeArray_()[bobby->getId()].isValid = false;
	}
	vecBobby.clear();
}

void Univers::delete_server() {
	if (serverTCP_) {
		serverTCP_ = nullptr;
		core_->addMessageChat(SuccessServerIsDelete);
		if (clientTCP_ && clientTCP_->isConnect())
			delete_client();
	} else {
		core_->addMessageChat(WarningServerNotExist);
	}
}

void Univers::delete_client() {
	if (clientTCP_) {
		clientTCP_ = nullptr;
		core_->addMessageChat(SuccessClientIsDelete);
	}
	else
		core_->addMessageChat(WarningClientNotExist);
}

void Univers::finish_game() {
	cleanAll();
}


/** Getter && Setter **/

MutantGrid<eSprite> &Univers::getGrid_() {
	return *grid_;
}

std::array<Snake, SNAKE_MAX> Univers::getSnakeArray_() const {
	if (isServer())
		return serverTCP_->getSnakeArray_();
	if (getSnakeClient())
		return getSnakeClient()->getSnakeArray_();
	return std::array<Snake, SNAKE_MAX>();
}


KINU::World &Univers::getWorld_() const {
	return *world_;
}

boost::shared_ptr<SnakeClient> Univers::getSnakeClient() const {
	if (clientTCP_)
		return clientTCP_->shared_from_this();
	else if (vecBobby.size() != 0)
		return vecBobby.front()->getClientTCP_()->shared_from_this();
	return nullptr;
}

ISound &Univers::getSound() const {
	return *sound;
}

Core &Univers::getCore_() const {
	return *core_;
}

unsigned int Univers::getMapSize() const {
	return mapSize_;
}

void Univers::setMapSize(unsigned int mapSize) {
	mapSize_ = mapSize;
	log_success("New map size [%d]", mapSize_);
}

void Univers::setBorderless(bool borderless) {
	Univers::borderless = borderless;
}

/** State **/

bool Univers::isServer() const {
	return serverTCP_ != nullptr;
}

bool Univers::isBorderless() const {
	return borderless;
}

bool Univers::isOnlyIA() const {
	return clientTCP_ == nullptr && vecBobby.size() != 0;
}

/** Sound **/

void Univers::addNoise(std::string const &path) {
	if (sound && flag_.test(eFlag::SOUND))
		sound->addNoise(path);
}
void Univers::playNoise(eNoise e) const {
	if (sound && flag_.test(eFlag::SOUND))
		sound->playNoise(static_cast<int>(e));
}
void Univers::playMusic(std::string const &path) const {
	if (sound && flag_.test(eFlag::SOUND)) {
		sound->setMusic(path.c_str());
		sound->playMusic();
	}
}


Core *Univers::releaseCore_() {
	return (core_.release());
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
	clientTCP_ = nullptr;
	serverTCP_ = nullptr;
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
	log_info("getSnakeClient(%d) && !getSnakeClient()->isConnect(%d)",
			getSnakeClient() != nullptr , getSnakeClient() ? !getSnakeClient()->isConnect() : -42);
	if (getSnakeClient() && !getSnakeClient()->isConnect()) {
		delete_server();
		delete_client();
		for (Snake &snake: getSnakeArray_()) {
			log_fatal("Snake [%d]isValid?[%d]", snake.id, snake.isValid);
		}
		borderless = false;
		mapSize_ = MAP_DEFAULT;
	}
	core_ = nullptr;
	grid_ = nullptr;
}
