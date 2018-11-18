#include "Univers.hpp"
#include <gui/Core.hpp>
#include <systems/MotionSystem.hpp>
#include <systems/JoystickSystem.hpp>
#include <systems/FollowSystem.hpp>
#include <systems/FoodSystem.hpp>
#include <systems/CollisionSystem.hpp>
#include <systems/SpriteSystem.hpp>
#include <systems/RenderSystem.hpp>
#include <network/ServerTCP.hpp>
#include <dlfcn.h>

Univers::Univers()
		: timer_start(boost::asio::deadline_timer(io_start)),
		  timer_loop(boost::asio::deadline_timer(io_loop,
												 boost::posix_time::milliseconds(
														 100))),
		  mapSize(0),
		  gameSpeed(100) {

	world_ = std::make_unique<KINU::World>(*this);
	core_ = nullptr; //std::make_unique<Core>(*this)
	clientTCP_ = ClientTCP::create(*this, io_client);
	serverTCP_ = nullptr;
	display = nullptr;
}


bool Univers::load_external_library(std::string title, std::string library_path) {

	if (display != nullptr && dlHandle != nullptr) {
		deleteDisplay(display);
		dlclose(dlHandle);
	}

	if (!(dlHandle = dlopen(library_path.c_str(), RTLD_LAZY | RTLD_LOCAL)))
		return (dlError());

	if (!(newDisplay = reinterpret_cast<IDisplay *(*)(
			const char *, int, int, int, const char *
	)>(dlsym(dlHandle, "newDisplay"))))
		return (dlError());

	if (!(deleteDisplay = reinterpret_cast<void (*)(
			IDisplay *
	)>(dlsym(dlHandle, "deleteDisplay"))))
		return (dlError());
	if (!(display = newDisplay(PATH_TILESET, DEFAULT_SIZE_SPRITE, mapSize,
							   mapSize, title.c_str())))
		return false;

	world_->grid = Grid<int>(mapSize);
	world_->grid.fill(SPRITE_GROUND);
	world_->grid.setBorder(SPRITE_WALL);
	display->setBackground(world_->grid);
	world_->setDisplay(display);

	display->update();
	display->render();
	return true;
}

void Univers::manage_input() {
	ClientTCP::InputInfo inputInfo;
	inputInfo.id = clientTCP_->getId();
	inputInfo.dir = display->getDirection();
	if (world_->getEntityManager().hasTag(Factory::factory_name(HEAD, inputInfo.id)))
		clientTCP_->write_socket(ClientTCP::add_prefix(INPUT, &inputInfo));
}

void Univers::manage_start() {
	ClientTCP::StartInfo startInfo;
	std::vector<StartEvent> startEvent;
	for (; startEvent.empty(); startEvent = world_->getEventManager().getEvents<StartEvent>());
	auto ptime = startEvent.front().start_time;
	timer_start.expires_at(ptime);
	io_start.run();
	timer_start.wait();
	timer_loop.expires_at(ptime + boost::posix_time::milliseconds(gameSpeed));
}


void Univers::loop() {
	world_->getSystemManager().addSystem<CollisionSystem>();
	world_->getSystemManager().addSystem<FollowSystem>();
	world_->getSystemManager().addSystem<FoodSystem>();
	world_->getSystemManager().addSystem<JoystickSystem>();
	world_->getSystemManager().addSystem<MotionSystem>();
	world_->getSystemManager().addSystem<SpriteSystem>();
	world_->getSystemManager().addSystem<RenderSystem>();

	world_->getEventManager().emitEvent<StartEvent>();
	world_->getEventManager().destroyEvents();
	manage_start();
	log_success("Univers::loop");

	timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
	boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_loop));
	thread.detach();

	world_->grid.clear();

	while (display == nullptr || !display->exit()) {
		if (display != nullptr) {
			display->update();
			manage_input();
			display->drawGrid(world_->grid);
			display->render();
		}
	}
}

void Univers::loop_world() {
	log_success("Univers::loop_world");

	clientTCP_->deliverEvents();

	world_->getSystemManager().getSystem<FollowSystem>().update();
	world_->getSystemManager().getSystem<JoystickSystem>().update();
	world_->getSystemManager().getSystem<MotionSystem>().update();
	world_->getSystemManager().getSystem<CollisionSystem>().update();
	world_->getSystemManager().getSystem<FoodSystem>().update();
	world_->getSystemManager().getSystem<SpriteSystem>().update();
	world_->getSystemManager().getSystem<RenderSystem>().update();

	world_->update();
	timer_loop.expires_at(
			timer_loop.expires_at() +
			boost::posix_time::milliseconds(gameSpeed));
	timer_loop.async_wait(boost::bind(&Univers::loop_world, this));
}

void Univers::create_ui() {
	core_ = std::make_unique<Core>(*this);
}

void Univers::create_server(unsigned int port) {
	serverTCP_ = std::make_unique<ServerTCP>(io_server, port);
	boost::thread t2(boost::bind(&boost::asio::io_service::run, &io_server));
	t2.detach();
}

KINU::World &Univers::getWorld_() const {
	return *world_;
}

ClientTCP &Univers::getClientTCP_() const {
	return *clientTCP_;
}


Core &Univers::getCore_() const {
	return *core_;
}

unsigned int Univers::getMapSize() {
	return mapSize;
}

bool Univers::dlError() {
	std::cerr << "Error : " << dlerror() << std::endl;
	return (false);
}

void Univers::setMapSize(unsigned int mapSize) {
	Univers::mapSize = mapSize;
}


