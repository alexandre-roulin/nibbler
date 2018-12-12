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
		  timer_loop(boost::asio::deadline_timer(io_loop)),
		  mapSize(MAP_DEFAULT),
		  gameSpeed(80),
		  dlHandleDisplay(nullptr),
		  dlHandleSound(nullptr),
		  display(nullptr),
		  sound(nullptr),
		  borderless(false) {

	clientTCP_ = ClientTCP::create(*this);
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
			const char *, int, int, int, const char *
	)>(dlsym(dlHandleDisplay, "newDisplay"))))
		return (dlError());

	if (!(deleteDisplay = reinterpret_cast<void (*)(
			IDisplay *
	)>(dlsym(dlHandleDisplay, "deleteDisplay"))))
		return (dlError());

	if (!(display = newDisplay(PATH_TILESET, DEFAULT_SIZE_SPRITE, mapSize,
							   mapSize, title.c_str())))
		return (false);

	if (world_ != nullptr)
		display->setBackground(world_->grid);

	if (world_ != nullptr)
		world_->setDisplay(display);

	display->update();
	display->render();
	return (true);
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

void Univers::unload_external_library() {
	log_error("Univers::unload_external_library");
	if (display != nullptr && dlHandleDisplay != nullptr) {
		if (deleteDisplay) {
			deleteDisplay(display);
			deleteDisplay = nullptr;
			newDisplay = nullptr;
		}
		dlclose(dlHandleDisplay);
	}}

/** Game Management **/

void Univers::new_game() {
	world_ = std::make_unique<KINU::World>(*this);
	world_->grid = Grid<int>(mapSize);
	world_->grid.fill(SPRITE_GROUND);
	world_->setDisplay(display);
	display->setBackground(world_->grid);
	loop();
}

void Univers::manage_input() {
	ClientTCP::InputInfo inputInfo;

	inputInfo.id = clientTCP_->getId();

	if (isIA())
		inputInfo.dir = bobby->getDirection();
	else
		inputInfo.dir = display->getDirection();

	assert(world_ != nullptr);
	if (world_->getEntitiesManager().hasEntityByTagId(eTag::HEAD_TAG + inputInfo.id))
		clientTCP_->write_socket(ClientTCP::add_prefix(INPUT, &inputInfo));
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

	timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
	thread = boost::thread(boost::bind(&boost::asio::io_service::run, &io_loop));
	thread.detach();

	world_->grid.clear();
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

	manage_input();

	for (; nextFrame.empty();) {
		clientTCP_->lock();
		nextFrame = world_->getEventsManager().getEvents<NextFrame>();

		clientTCP_->unlock();
	}
	nextFrame.clear();
	world_->getEventsManager().destroy<NextFrame>();

	clientTCP_->deliverEvents();

	world_->update();

	world_->getSystemsManager().getSystem<FollowSystem>().update();
	world_->getSystemsManager().getSystem<JoystickSystem>().update();
	world_->getEventsManager().destroy<JoystickEvent>();
	world_->getSystemsManager().getSystem<MotionSystem>().update();

	if (isIA()) {
		boost::thread t2(boost::bind(&Bobby::calculateDirection, bobby.get()));
		t2.detach();
	}

	world_->getSystemsManager().getSystem<CollisionSystem>().update();
	world_->getSystemsManager().getSystem<FoodCreationSystem>().update();
	world_->getEventsManager().destroy<FoodCreation>();
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
	world_->getSystemsManager().getSystem<RenderSystem>().update();
	world_->getSystemsManager().getSystem<FoodEatSystem>().update();
	world_->getEventsManager().destroy<FoodEat>();

	if (!clientTCP_->all_snake_is_dead()) {
		timer_loop.expires_at(
				timer_loop.expires_at() +
				boost::posix_time::milliseconds(gameSpeed));
		timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
	}
}

/** Create and delete **/

void Univers::create_ui() {
	core_ = std::make_unique<Core>(*this);
}

void Univers::create_server(unsigned int port) {
	serverTCP_ = std::make_unique<ServerTCP>(port);
}

void Univers::create_ia() {
	if (bobby == nullptr)
		bobby = std::make_unique<Bobby>(*this);
}

void Univers::delete_ia() {
	bobby = nullptr;
}

void Univers::delete_server() {
	if (serverTCP_ != nullptr) {
		serverTCP_ = nullptr;
		delete_client();
	}
}

void Univers::delete_client() {
	clientTCP_ = ClientTCP::create(*this);
}

/** Getter && Setter **/

KINU::World &Univers::getWorld_() const {
	return *world_;
}

ClientTCP &Univers::getClientTCP_() const {
	return *clientTCP_;
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

bool Univers::isIA() const {
	return bobby != nullptr;
}

bool Univers::isBorderless() const {
	return borderless;
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

bool Univers::dlError() {
	std::cerr << "Error : " << dlerror() << std::endl;
	dlclose(dlHandleDisplay);
	return (false);
}












