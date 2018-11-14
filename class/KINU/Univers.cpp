#include "Univers.hpp"
#include <gui/Core.hpp>
#include <dlfcn.h>
#include <systems/MotionSystem.hpp>
#include <systems/JoystickSystem.hpp>
#include <systems/FollowSystem.hpp>
#include <systems/FoodSystem.hpp>
#include <systems/CollisionSystem.hpp>
#include <systems/RenderSystem.hpp>
#include <boost/thread.hpp>
#include <events/StartEvent.hpp>
#include <logger.h>
#include <network/ClientTCP.hpp>
#include <network/ServerTCP.hpp>

Univers::Univers() {

	world_ = std::make_unique<Mix::World>(*this);
	core_ = nullptr;
	clientTCP_ = nullptr;
	serverTCP_ = nullptr;
	display = nullptr;
}


int Univers::start_game() {
	if (!(dlHandle = dlopen("./externlib/display_sfml/display_sfml.so",
							RTLD_LAZY | RTLD_LOCAL)))
		return (dlError());
	if (!(newDisplay = reinterpret_cast<IDisplay *(*)(
			const char *, int, int, int, const char *
	)>(dlsym(dlHandle, "newDisplay"))))
		return (dlError());
	if (!(deleteDisplay = reinterpret_cast<void (*)(
			IDisplay *
	)>(dlsym(dlHandle, "deleteDisplay"))))
		return (dlError());
	display = newDisplay(PATH_TILESET, DEFAULT_SIZE_SPRIT, 30, 30, "Nibblour");
	assert(display != nullptr);
	Grid<int> grid(30, 30);

	grid.fill(SPRITE_GROUND);
	grid.setBorder(SPRITE_WALL);
	display->setBackground(grid);
	world_->setDisplay(display);
	display->update();
	display->render();
	return 0;
}

void Univers::manage_input() {
	eDirection ed = display->getDirection();
	int16_t id = clientTCP_->getId_();
	if (world_->getEntityManager().hasTag(Factory::factory_name(HEAD, id)))
		clientTCP_->write_socket(ClientTCP::add_prefix(INPUT, &id, &ed));
}

void Univers::loop() {
	world_->getSystemManager().addSystem<CollisionSystem>();
	world_->getSystemManager().addSystem<FollowSystem>();
	world_->getSystemManager().addSystem<FoodSystem>();
	world_->getSystemManager().addSystem<JoystickSystem>();
	world_->getSystemManager().addSystem<MotionSystem>();
	world_->getSystemManager().addSystem<RenderSystem>();

	world_->getEventManager().emitEvent<StartEvent>();
	world_->getEventManager().destroyEvents();
	while (world_->getEventManager().getEvents<StartEvent>().empty());
	log_success("Univers::loop");

	world_->update();
	deadline_timer.async_wait(boost::bind(&Univers::loop_world, this));
	boost::thread thread(boost::bind(&boost::asio::io_service::run, &io));
	thread.detach();

	world_->grid.clear();

	std::cout << "Bug Display"  << std::endl;
	while (!display->exit()) {
		display->update();
		manage_input();
		display->drawGrid(world_->grid);
		display->render();
	}
}


void Univers::loop_world() {
	log_success("Univers::loop_world");
	world_->grid.clear();

	world_->getSystemManager().getSystem<FollowSystem>().update();
	world_->getSystemManager().getSystem<JoystickSystem>().update();
	world_->getSystemManager().getSystem<MotionSystem>().update();
	world_->getSystemManager().getSystem<CollisionSystem>().update();
	world_->getSystemManager().getSystem<FoodSystem>().update();
	world_->getSystemManager().getSystem<RenderSystem>().update();

	deadline_timer.expires_at(deadline_timer.expires_at() + boost::posix_time::milliseconds(350));
	deadline_timer.async_wait(boost::bind(&Univers::loop_world, this));
	world_->update();

}

Mix::World &Univers::getWorld_() const {
	return *world_;
}

ClientTCP &Univers::getClientTCP_() const {
	return *clientTCP_;
}

ServerTCP &Univers::getServerTCP_() const {
	return *serverTCP_;
}


Core &Univers::getCore_() const {
	return *core_;
}

void Univers::create_ui() {
	core_ = std::make_unique<Core>(*this);
}

void Univers::create_server() {
	serverTCP_ = std::make_unique<ServerTCP>(*this, io_server);
	boost::thread t2(boost::bind(&boost::asio::io_service::run, &io_server));
	t2.detach();
}

void Univers::create_client() {
	clientTCP_ = ClientTCP::create(*this, io_client,
								   std::string("localhost"));
	clientTCP_->connect();
	clientTCP_->read_socket_header();
	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_client));
	t.detach();
}

bool Univers::dlError() {
	std::cerr << "Error : " << dlerror() << std::endl;
	return (false);
}

IDisplay &Univers::getDisplay() const {
	assert(display != nullptr);
	return *display;
}
