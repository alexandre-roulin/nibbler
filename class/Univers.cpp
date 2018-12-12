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
#include <dlfcn.h>

Univers::Univers()
		: timer_start(boost::asio::deadline_timer(io_start)),
		  timer_loop(boost::asio::deadline_timer(io_loop,
												 boost::posix_time::milliseconds(
														 100))),
		  mapSize(MAP_DEFAULT),
		  gameSpeed(80),
		  dlHandleDisplay(nullptr),
		  dlHandleSound(nullptr),
		  display(nullptr),
		  sound(nullptr),
		  borderless(false) {

	world_ = nullptr;
	core_ = nullptr; //std::make_unique<Core>(*this)
	clientTCP_ = ClientTCP::create(*this, io_client);
	serverTCP_ = nullptr;
}

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
		return (dlError());

	if (!(newSound = reinterpret_cast<ISound *(*)(
			const char *) >(dlsym(dlHandleSound, "newSound"))))
		return (dlError());

	if (!(deleteSound = reinterpret_cast<void (*)(
			ISound *)>(dlsym(dlHandleSound, "deleteSound"))))
		return (dlError());
	return (sound = newSound(library_path.c_str())) != nullptr;
}

bool Univers::load_external_display_library(std::string const &title,
											std::string const &libPath) {

	if (!(dlHandleDisplay = dlopen(libPath.c_str(), RTLD_LAZY | RTLD_LOCAL)))
		return (dlError());

	if (!(newDisplay = reinterpret_cast<IDisplay *(*)(
			int, int, const char *
	)>(dlsym(dlHandleDisplay, "newDisplay"))))
		return (dlError());

	if (!(deleteDisplay = reinterpret_cast<void (*)(
			IDisplay *
	)>(dlsym(dlHandleDisplay, "deleteDisplay"))))
		return (dlError());

	if (!(display = newDisplay(mapSize, mapSize, title.c_str())))
		return (false);

	if (world_ != nullptr)
		display->setBackground(world_->grid);

	if (world_ != nullptr)
		world_->setDisplay(display);

	display->update();
	display->render();
	return (true);
}

void Univers::manage_input() {
	ClientTCP::InputInfo inputInfo;


	inputInfo.id = clientTCP_->getId();
	inputInfo.dir = display->getDirection();

	assert(world_ != nullptr);
	if (world_->getEntitiesManager().hasEntityByTagId(
			eTag::HEAD_TAG + inputInfo.id))
		clientTCP_->write_socket(ClientTCP::add_prefix(eHeader::INPUT, &inputInfo));

	return;
	if (clientTCP_->getId() == 0) {
		inputInfo.dir = display->getDirection();

		for (int j = 0; j < 5; ++j) {
			inputInfo.id = j;
			if (world_->getEntitiesManager().hasEntityByTagId(
					eTag::HEAD_TAG + inputInfo.id))
				clientTCP_->write_socket(
						ClientTCP::add_prefix(eHeader::INPUT, &inputInfo));
		}
	}
}

void Univers::manage_start() {
	ClientTCP::StartInfo startInfo;
	std::vector<StartEvent> startEvent;
	for (; startEvent.empty();) {
		clientTCP_->lock();
		startEvent = world_->getEventsManager().getEvents<StartEvent>();
		clientTCP_->unlock();
	}
	auto ptime = startEvent.front().start_time;
	timer_start.expires_at(ptime);
	io_start.run();
	timer_start.wait();
	timer_loop.expires_at(ptime + boost::posix_time::milliseconds(gameSpeed));
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

	log_success("Univers::async_wait");
	timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
	thread = boost::thread(boost::bind(&boost::asio::io_service::run, &io_loop));
	log_success("Univers::detach");
	thread.detach();
	log_success("Univers::loop");

	world_->grid.fill(eSprite::NONE);
	playMusic("./ressource/sound/zelda.ogg");

	while ((display == nullptr || !display->exit()) && !clientTCP_->all_snake_is_dead()) {
		if (display != nullptr) {
			display->update();
			display->drawGrid(world_->grid); //TODO REFRESH WITH CACHE
			display->render();
		}
	}
	unload_external_library();
	world_ = nullptr;
}

void Univers::loop_world() {


	// SEND DIRECTION
	manage_input();
	std::cout << "manage_input" << std::endl;

	// GET REFRESH DATA
	std::cout << "NextFrame" << (nextFrame.empty() ? "Empty" : "Not empty ?!")
			  << std::endl;
	for (; nextFrame.empty();) {
		clientTCP_->lock();
		nextFrame = world_->getEventsManager().getEvents<NextFrame>();

		clientTCP_->unlock();
	}
	std::cout << "NextFrame.clear()" << std::endl;
	nextFrame.clear();
	std::cout << "getEventsManager" << std::endl;
	world_->getEventsManager().destroy<NextFrame>();

	std::cout << "deliverEvents" << std::endl;
	clientTCP_->deliverEvents();

	std::cout << "update" << std::endl;
	world_->update();
	std::cout << "getSystemsManager" << std::endl;

	world_->getSystemsManager().getSystem<FollowSystem>().update();
	world_->getSystemsManager().getSystem<JoystickSystem>().update();
	world_->getEventsManager().destroy<JoystickEvent>();
	world_->getSystemsManager().getSystem<MotionSystem>().update();
	world_->getSystemsManager().getSystem<CollisionSystem>().update();
	world_->getSystemsManager().getSystem<FoodCreationSystem>().update();
	world_->getEventsManager().destroy<FoodCreation>();
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
	world_->getSystemsManager().getSystem<RenderSystem>().update();
	world_->getSystemsManager().getSystem<FoodEatSystem>().update();
	world_->getEventsManager().destroy<FoodEat>();

	std::cout << "timer_loop" << std::endl;
	if (!clientTCP_->all_snake_is_dead()) {
		timer_loop.expires_at(
				timer_loop.expires_at() +
				boost::posix_time::milliseconds(gameSpeed));
		timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
	}
}

void Univers::create_ui() {
	core_ = std::make_unique<Core>(*this);
}

void Univers::create_server(unsigned int port) {
	serverTCP_ = std::make_unique<ServerTCP>(io_server, port);
	boost::thread t2(boost::bind(&boost::asio::io_service::run, &io_server));
	t2.detach();
}

bool Univers::isServer() const {
	return serverTCP_ != nullptr;
}

KINU::World &Univers::getWorld_() const {
	return *world_;
}

ClientTCP &Univers::getClientTCP_() const {
	return *clientTCP_;
}

ISound &Univers::getSound() const {
	return *sound;
}


void Univers::playNoise(int i) const {
	if (sound && flag.test(eFlag::SOUND))
		sound->playNoise(i);
}

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

void Univers::playMusic(char *path) const {
	if (sound && flag.test(eFlag::SOUND)) {
		sound->setMusic(path);
		sound->playMusic();
	}
}

Core &Univers::getCore_() const {
	return *core_;
}

Core *Univers::releaseCore_() {
	return (core_.release());
}

unsigned int Univers::getMapSize() const {
	return mapSize;
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


bool Univers::dlError() {
	std::cerr << "Error : " << dlerror() << std::endl;
	dlclose(dlHandleDisplay);
	return (false);
}

void Univers::setMapSize(unsigned int mapSize) {
	log_success("New map size [%d]", mapSize);
	Univers::mapSize = mapSize;
}

bool Univers::isBorderless() const {
	return borderless;
}

void Univers::setBorderless(bool borderless) {
		Univers::borderless = borderless;
}

bool Univers::load_extern_lib_display(Univers::eDisplay eLib) {
	switch (eLib) {
		case EXTERN_LIB_SFML : {
			return load_external_display_library(std::string("Nibbler - SFML"),
										  std::string(PATH_DISPLAY_LIBRARY_SFML));
		}
		case EXTERN_LIB_SDL : {
			// TODO ADD SDL
			break;
		}
		case EXTERN_LIB_STB : {
			// TODO ADD STB
			break;
		}
	}
	return false;
}

void Univers::new_game() {
	world_ = std::make_unique<KINU::World>(*this);
	world_->grid = Grid< eSprite >(mapSize);
	world_->grid.fill(eSprite::GROUND);
	world_->setDisplay(display);
	display->setBackground(world_->grid);
	loop();
}

void Univers::unload_external_library() {
	log_error("Univers::unload_external_library");
	if (display != nullptr && dlHandleDisplay != nullptr) {
		display->exit();
		if (deleteDisplay) {
			deleteDisplay(display);
			deleteDisplay = nullptr;
			newDisplay = nullptr;
		}
		dlclose(dlHandleDisplay);
	}}


