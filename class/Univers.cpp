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
#include <KINU/World.hpp>
#include <events/StartEvent.hpp>
#include <events/FoodCreation.hpp>
#include <events/JoystickEvent.hpp>

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

	clientTCP_ = ClientTCP::create(*this, false);
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
			const char *, int, int, int, const char *
	)>(dlsym(dlHandleDisplay, "newDisplay"))))
		return (dlError("dlsym"));

	if (!(deleteDisplay = reinterpret_cast<void (*)(
			IDisplay *
	)>(dlsym(dlHandleDisplay, "deleteDisplay"))))
		return (dlError("dlsym"));

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
												 std::string(
														 PATH_DISPLAY_LIBRARY_SFML));
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
	}
}

/** Game Management **/

void Univers::new_game() {
	world_ = std::make_unique<KINU::World>(*this);
	world_->grid = Grid<int>(mapSize);
	world_->grid.fill(SPRITE_GROUND);
	world_->setDisplay(display);
	display->setBackground(world_->grid);
	if (isServer()) {
		for (auto &bobby : vecBobby) {
			bobby->setGrid_(&world_->grid);
			bobby->buildIA();
		}
	}
	loop();
}

void Univers::manage_input() {
	ClientTCP::InputInfo inputInfo;


	if (isServer()) {
		for (auto &bobby : vecBobby) {
			bobby->sendDirection();
		}
	}


	inputInfo.id = clientTCP_->getId();
	inputInfo.dir = display->getDirection();
	assert(world_ != nullptr);
	if (world_->getEntitiesManager().hasEntityByTagId(
			eTag::HEAD_TAG + inputInfo.id)) {
		clientTCP_->write_socket(ClientTCP::add_prefix(INPUT, &inputInfo));
	}
}

void Univers::manage_start() {
	ClientTCP::StartInfo startInfo;
	std::vector <StartEvent> startEvent;
	for (; startEvent.empty();) {
		std::cout << "stuck" << std::endl;
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
	thread = boost::thread(
			boost::bind(&boost::asio::io_service::run, &io_loop));
	thread.detach();

	world_->grid.clear();
	playMusic("./ressource/sound/zelda.ogg");

	while ((display == nullptr || !display->exit()) &&
		   !clientTCP_->all_snake_is_dead()) {
		if (display != nullptr) {
			display->update();
			display->drawGrid(world_->grid);
			display->render();
		}
	}
	unload_external_library();
	finish_game();
}

void Univers::loop_world() {

	manage_input();
	int i = 0;

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

	if (isServer()) {
		for (auto &bobby : vecBobby) {
			if (world_->getEntitiesManager().hasEntityByTagId(bobby->getId() + eTag::HEAD_TAG)) {
				boost::thread
						t2(boost::bind(&Bobby::calculateDirection, bobby.get()));
				t2.detach();
			}

		}
	}
	world_->getSystemsManager().getSystem<CollisionSystem>().update();
	world_->getSystemsManager().getSystem<FoodCreationSystem>().update();
	world_->getEventsManager().destroy<FoodCreation>();
	world_->getSystemsManager().getSystem<SpriteSystem>().update();
	world_->getSystemsManager().getSystem<RenderSystem>().update();
	world_->getSystemsManager().getSystem<FoodEatSystem>().update();
	world_->getEventsManager().destroy<FoodEat>();

	if (!clientTCP_->all_snake_is_dead()) {
//		std::cout << "Recursive" << std::endl;

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
	if (isServer() && !serverTCP_->isFull()) {
		ClientTCP::pointer_client clientTCPIA = ClientTCP::create(*this, true);
		clientTCPIA->connect(clientTCP_->getDns(), clientTCP_->getPort());
		sleep(1);
		std::cout << clientTCPIA->getId() << std::endl;
		clientTCPIA->change_state_ready();
		vecBobby.push_back(
				std::make_unique<Bobby>(*this, clientTCPIA));
	}
}

void Univers::delete_ia() {
//	bobby = nullptr;
}

void Univers::delete_server() {
	if (serverTCP_ != nullptr) {
		serverTCP_ = nullptr;
		delete_client();
	}
}

void Univers::delete_client() {
	clientTCP_ = ClientTCP::create(*this, false);
}

void Univers::finish_game() {
	thread.interrupt();
	io_loop.reset();
	world_ = nullptr;
}


void Univers::close_acceptor() {
	serverTCP_->close_acceptor();
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












