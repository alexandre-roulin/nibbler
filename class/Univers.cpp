#include "Univers.hpp"
#include <gui/Core.hpp>
#include <systems/MotionSystem.hpp>
#include <systems/JoystickSystem.hpp>
#include <systems/FollowSystem.hpp>
#include <systems/FoodCreationSystem.hpp>
#include <systems/CollisionSystem.hpp>
#include <systems/SpriteSystem.hpp>
#include <systems/RenderSystem.hpp>
#include <network/ServerTCP.hpp>
#include <systems/FoodEatSystem.hpp>
#include <events/FoodEat.hpp>
#include <KINU/World.hpp>
#include <events/StartEvent.hpp>
#include <events/FoodCreation.hpp>
#include <events/JoystickEvent.hpp>
#include <dlfcn.h>

/** Const Variable **/

const std::string Univers::WarningServerCreateIA = "Only the server owner can create IA";
const std::string Univers::WarningServerFull = "Server have reach the maximum player";
const std::string Univers::WarningServerIsUp = "Server is already in place";
const std::string Univers::SuccessServerIsCreate = "Server is up.";
const std::string Univers::WarningClientExist = "Client is already in place.";

Univers::Univers()
		: timer_start(boost::asio::deadline_timer(io_start)),
		  timer_loop(boost::asio::deadline_timer(io_loop)),
		  mapSize(MAP_DEFAULT),
		  gameSpeed(80),
		  dlHandleDisplay(nullptr),
		  dlHandleSound(nullptr),
		  display(nullptr),
		  sound(nullptr),
		  borderless(false),
		  kDisplay(eDisplay::kExternSfmlLibrary) {
	switchLib = false;
	clientTCP_ = nullptr;
	serverTCP_ = nullptr;
	world_ = nullptr;
	core_ = nullptr; //std::make_unique<Core>(*this)
}

/** External Library Management **/


bool Univers::load_external_sound_library(std::string const &title,
										  std::string const &library_path) {
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

	if (!(display = newDisplay(mapSize, mapSize, title.c_str())))
		return (false);

	log_success("load_external_display_library up !");
	return (true);
}

bool Univers::load_extern_lib_display(Univers::eDisplay eLib) {
	switch (eLib) {
		case kExternSfmlLibrary : {
			log_success("Univers::load_extern_lib_display.kExternSfmlLibrary");
			return load_external_display_library(
					std::string("Nibbler - SFML"),
					std::string(PATH_DISPLAY_LIBRARY_SFML)
					);
		}
		case kExternSdlLibrary : {
			log_success("Univers::load_extern_lib_display.kExternSdlLibrary");
			return load_external_display_library(
					std::string("Nibbler - SDL"),
					std::string(PATH_DISPLAY_LIBRARY_SDL)
			);		}
		case kExternGlfwLibrary : {
			log_success("Univers::load_extern_lib_display.kExternGlfwLibrary");
			return load_external_display_library(
					std::string("Nibbler - GLFW"),
					std::string(PATH_DISPLAY_LIBRARY_GLFW)
			);		}
	}
	return false;
}

void Univers::unload_external_library() {
	log_error("Univers::unload_external_library");
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
	display = nullptr;
	log_error("Univers::unload_external_library.unlock()");
}

/** Game Management **/


void Univers::defaultAssignmentLibrary() {
	if (!display) return;

	MutantGrid<eSprite> grid(mapSize);
	grid.fill(eSprite::GROUND);
	display->setBackground(grid);
	display->registerCallbackAction(std::bind(&Univers::callbackAction, this, std::placeholders::_1));
	display->update(0.17f);
	display->render(1.f, 1.f);
}

void Univers::new_game() {

	if (isServer()) {
		for (auto &bobby : vecBobby) {
			bobby->getClientTCP_()->change_state_ready();
		}
	}
	sleep(1);
	assert(isServer());
	assert(serverTCP_->isReady());
	if (!isServer() || !serverTCP_->isReady()) return;
	world_ = std::make_unique<KINU::World>(*this);
	world_->grid.resize(mapSize);
	if (!display) load_extern_lib_display(kDisplay);
	defaultAssignmentLibrary();
	if (isServer()) {
		for (auto &bobby : vecBobby) {
//			bobby->getClientTCP_()->change_state_ready();
			bobby->buildIA();
			bobby->sendDirection();
		}
		serverTCP_->start_game();
	}

	loop();
}

void Univers::manage_input() {

	if (clientTCP_) {
		ClientTCP::InputInfo inputInfo;
		inputInfo.id = clientTCP_->getId();
		if (display) {
			inputInfo.dir = display->getDirection();
		}
		else
			inputInfo.dir = eDirection::kNorth;
		if (world_->getEntitiesManager().hasEntityByTagId(
				eTag::HEAD_TAG + inputInfo.id)) {
			clientTCP_->write_socket(
					ClientTCP::add_prefix(eHeader::INPUT, &inputInfo));
		}
	}

}

void Univers::manage_start() {
	ClientTCP::StartInfo startInfo;
	std::vector<StartEvent> startEvent;
	for (; startEvent.empty();) {

		getMainClientTCP()->lock();
		startEvent = world_->getEventsManager().getEvents<StartEvent>();
		getMainClientTCP()->unlock();
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

	world_->getSystemsManager().addSystem<CollisionSystem>();
	world_->getSystemsManager().addSystem<FollowSystem>();
	world_->getSystemsManager().addSystem<JoystickSystem>();
	world_->getSystemsManager().addSystem<MotionSystem>();
	world_->getSystemsManager().addSystem<SpriteSystem>();
	world_->getSystemsManager().addSystem<RenderSystem>();
	world_->getSystemsManager().addSystem<FoodCreationSystem>();
	world_->getSystemsManager().addSystem<FoodEatSystem>();

	manage_start();

	timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
	thread = boost::thread(
			boost::bind(&boost::asio::io_service::run, &io_loop));
	thread.detach();

	world_->grid.fill(eSprite::NONE);
	playMusic("./ressource/sound/zelda.ogg");



	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds current(0);

	while ((display == nullptr || !display->exit()) &&
		   !getMainClientTCP()->all_snake_is_dead()) {
		if (switchLib)
			manageSwitchLibrary();
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		t1 = t2;
		current += time_span;

		if (current > std::chrono::milliseconds(gameSpeed))
			current = std::chrono::milliseconds(gameSpeed);

		//std::cout << "It took me " << current.count() << "," << gameSpeed << " seconds." << std::endl;
		display->update(0.17f);
		display->drawGrid(world_->grid);
		display->render(1.f, 1.f);
//			display->update(0.2f);
//			display->drawGrid(world_->grid);
			//display->render(current.count(), gameSpeed);
//			display->render(gameSpeed, gameSpeed);
		if (current >= std::chrono::milliseconds(gameSpeed)) {
			current = std::chrono::milliseconds(0);
		}
	}
	unload_external_library();
	exit(0);
	finish_game();
}

void Univers::loop_world() {

	manage_input();

	for (; nextFrame.empty();) {
		getMainClientTCP()->lock();
		// std::cout << "Stuck nextFrame" << std::endl;
		nextFrame = world_->getEventsManager().getEvents<NextFrame>();
		getMainClientTCP()->unlock();
	}
	nextFrame.clear();
	world_->getEventsManager().destroy<NextFrame>();

	getMainClientTCP()->deliverEvents();

	world_->update();

	//log_info("Univers::FollowSystem");
	world_->getSystemsManager().getSystem<FollowSystem>().update();
	//log_info("Univers::JoystickSystem");
	world_->getSystemsManager().getSystem<JoystickSystem>().update();
	//log_info("Univers::JoystickEvent");
	world_->getEventsManager().destroy<JoystickEvent>();
	//log_info("Univers::MotionSystem");
	world_->getSystemsManager().getSystem<MotionSystem>().update();
	//log_info("Univers::CollisionSystem");
	world_->getSystemsManager().getSystem<CollisionSystem>().update();

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

	//log_info("Univers::FoodCreationSystem");
	world_->getSystemsManager().getSystem<FoodCreationSystem>().update();
	world_->getEventsManager().destroy<FoodCreation>();
	//log_info("Univers::SpriteSystem");
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
	//log_info("Univers::RenderSystem");
	world_->getSystemsManager().getSystem<RenderSystem>().update();
	//log_info("Univers::FoodEatSystem");

	//log_info("Univers::vecBobby");


	world_->getSystemsManager().getSystem<FoodEatSystem>().update();
	world_->getEventsManager().destroy<FoodEat>();

	if (!getMainClientTCP()->all_snake_is_dead()) {

		timer_loop.expires_at(
				timer_loop.expires_at() +
				boost::posix_time::milliseconds(gameSpeed));
		timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
	}
//	world_->grid.print();
}

void Univers::manageSwitchLibrary() {
	if (!getGameNetwork()->isSwitchingLibrary()) {
		int16_t id = getGameNetwork()->getId();
		getGameNetwork()->write_socket(ClientTCP::add_prefix(eHeader::kForcePause, &id));
//		kDisplay = (kDisplay == kExternSdlLibrary) ? kExternSfmlLibrary : kExternSdlLibrary;
		kDisplay = (kDisplay == kExternGlfwLibrary) ? kExternSfmlLibrary : static_cast<eDisplay>(kDisplay + 1);
		unload_external_library();
		load_extern_lib_display(kDisplay);
		defaultAssignmentLibrary();
		getGameNetwork()->write_socket(ClientTCP::add_prefix(eHeader::kForcePause, &id));
	}
	switchLib = false;
}

void Univers::refreshTimerLoopWorld() {
	timer_loop.expires_at(boost::posix_time::microsec_clock::universal_time());
}


void Univers::callbackAction(eAction action) {
	if (getGameNetwork() == nullptr) return;
	switch (action) {
		case eAction::kPause :
			log_success(" eAction::kPause");
			getGameNetwork()->write_socket(ClientTCP::add_prefix<eAction>(eHeader::kPause, &action));
			break;
		case eAction::kSwitchDisplayLibrary:
			log_success("eAction::kSwitchDisplayLibrary");
			switchLib = true;
			break;
	}
}

/** Create and delete **/


void Univers::create_client() {
	log_error("%d", clientTCP_.get() == nullptr);
	if (!clientTCP_)
		clientTCP_ = std::make_unique<ClientTCP>(*this, false);
	else
		core_->addMessageChat(WarningClientExist);
}

void Univers::create_ui() {
	core_ = std::make_unique<Core>(*this);
}

void Univers::create_server(unsigned int port) {
	if (serverTCP_)
		core_->addMessageChat(WarningServerIsUp);
	else {
		serverTCP_ = std::make_unique<ServerTCP>(*this, port);
		core_->addMessageChat(SuccessServerIsCreate);
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
}

void Univers::delete_ia() {
//	bobby = nullptr;
}

void Univers::delete_server() {
	if (serverTCP_) {
		if (clientTCP_->isConnect())
			clientTCP_ = nullptr;
		serverTCP_ = nullptr;
		delete_client();
	}
}

void Univers::delete_client() {
	clientTCP_.reset();
}

void Univers::finish_game() {
	thread.interrupt();
	io_loop.stop();
	world_ = nullptr;
}


void Univers::close_acceptor() {
	serverTCP_->close_acceptor();
}

/** Getter && Setter **/

ClientTCP *Univers::getMainClientTCP() const {
//	std::cout << "Univers::getMainClientTCP[C:" <<  (clientTCP_ != nullptr) << "-B:" << (vecBobby.size() != 0 ) << "]" << std::endl;
	if (clientTCP_)
		return clientTCP_.get();
	else if (vecBobby.size() != 0)
		return vecBobby.front()->getClientTCP_();
	return nullptr;

}

KINU::World &Univers::getWorld_() const {
	return *world_;
}

IGameNetwork *Univers::getGameNetwork() const {
	if (clientTCP_)
		return clientTCP_.get();
	else if (vecBobby.size() != 0) {
		return vecBobby.front()->getClientTCP_();
	}
	return nullptr;
}

ISound &Univers::getSound() const {
	return *sound;
}

Core &Univers::getCore_() const {
	return *core_;
}

unsigned int Univers::getMapSize() const {
	return mapSize;
}

void Univers::setMapSize(unsigned int mapSize) {
	log_success("New map size [%d]", mapSize);
	Univers::mapSize = mapSize;
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

void Univers::playNoise(eSound e) const {
	if (sound && flag.test(eFlag::SOUND))
		sound->playNoise(static_cast<int>(e));
}


void Univers::playMusic(std::string const &path) const {
	if (sound && flag.test(eFlag::SOUND)) {
		sound->setMusic(path.c_str());
		sound->playMusic();
	}
}


Core *Univers::releaseCore_() {
	return (core_.release());
}

void Univers::setFlag(eFlag flag) {
	this->flag.set(flag);
}

void Univers::unsetFlag(eFlag flag) {
	this->flag.reset(flag);
}


bool Univers::testFlag(eFlag flag) {
	return (this->flag.test(flag));
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



