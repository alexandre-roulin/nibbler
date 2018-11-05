#include "Univers.hpp"
#include <gui/Core.hpp>
#include <dlfcn.h>
#include <systems/MotionSystem.hpp>
#include <systems/JoystickSystem.hpp>
#include <systems/FollowSystem.hpp>
#include <systems/FoodSystem.hpp>
#include <systems/CollisionSystem.hpp>
#include <systems/RenderSystem.hpp>

Univers::Univers() {

	world_ = std::make_unique<KNU::World>(*this);
	core_ = nullptr;
	clientTCP_ = nullptr;
	serverTCP_ = nullptr;
	display = nullptr;

	int count = 0;
}


int Univers::start_game() {
	if (!(dlHandle = dlopen("./externlib/display_sdl/display_sdl.so",
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
	std::cout << "dlfini" << std::endl;
	display = newDisplay(PATH_TILESET, DEFAULT_SIZE_SPRIT, 30, 30, "Nibblour");
	assert(display != nullptr);
	Grid<int> grid(30, 30);

	grid.fill(SPRITE_GROUND);
	grid.setBorder(SPRITE_WALL);
	display->setBackground(grid);
	world_->setDisplay(display);
	return 0;
}

void Univers::manage_input() {
	char des[ClientTCP::size_header[INPUT]];
	std::string buffer;
	eDirection ed = display->getDirection();
	int16_t id = clientTCP_->getId_();
	std::memcpy(des, &id, sizeof(int16_t));
	std::memcpy(des, &ed, sizeof(eDirection));
	ClientTCP::add_prefix(INPUT, buffer, des, ClientTCP::size_header[INPUT]);
	serverTCP_->async_write(buffer);
}

void Univers::loop() {

	world_->getSystemManager().addSystem<CollisionSystem>();
	world_->getSystemManager().addSystem<FollowSystem>();
	world_->getSystemManager().addSystem<FoodSystem>();
	world_->getSystemManager().addSystem<JoystickSystem>();
	world_->getSystemManager().addSystem<MotionSystem>();
	world_->getSystemManager().addSystem<RenderSystem>();

	deadline_timer.async_wait(boost::bind(&Univers::loop_world, this));

	for (;;) {
		display->update();
		manage_input();
		display->render();
	}
}


void Univers::loop_world() {
	world_->update();

	world_->getSystemManager().getSystem<FollowSystem>()->update();
	world_->getSystemManager().getSystem<JoystickSystem>()->update();
	world_->getSystemManager().getSystem<MotionSystem>()->update();
	world_->getSystemManager().getSystem<CollisionSystem>()->update();
	world_->getSystemManager().getSystem<FoodSystem>()->update();
//	world_->getSystemManager().getSystem<RenderSystem>()->update();

	deadline_timer.expires_at(deadline_timer.expires_at() + boost::posix_time::seconds(1));
	deadline_timer.async_wait(boost::bind(&Univers::loop_world, this));

}

KNU::World &Univers::getWorld_() const {
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
	std::string buffer;
	std::cout << "Connect : ";
	std::getline(std::cin, buffer);
	clientTCP_ = ClientTCP::create(*this, io_client,
								   std::string(buffer.c_str()));
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
