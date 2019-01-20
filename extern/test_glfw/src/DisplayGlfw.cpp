#include <fstream>
#include <algorithm>
#include <memory>
#include <stb_image.h>
#include "DisplayGlfw.hpp"
#include "nibbler.hpp"
#include "Skybox.hpp"

IDisplay *newDisplay(int width,
                     int height,
                     char const *windowName) {
    return (new DisplayGlfw(width, height, windowName));
}

void deleteDisplay(IDisplay *display) {
    delete display;
}

DisplayGlfw::DisplayGlfw(int width,
                         int height,
                         char const *windowName) :
Glfw(windowName, DISPLAY_GLFW_WIN_WIDTH, DISPLAY_GLFW_WIN_HEIGHT),
direction_(NORTH),
currentTimer_(0.f),
maxTimer_(0.f),
winTileSize_(Vector2D<int>(width, height)),
tileBackground_(winTileSize_.getX(), winTileSize_.getY()),
background_(winTileSize_.getX(), winTileSize_.getY()),
tileGrid_(winTileSize_.getX(), winTileSize_.getY()),
grid_(winTileSize_.getX(), winTileSize_.getY()),
testParticle_(nullptr),
deltaTime_(0.016f),
skybox_(nullptr),
projection_(1.f),
view_(1.f),
model_(1.f) {

	std::cout << "Debug : 1" << std::endl;
	getPath_();

    glfwSetCursorPosCallback(getWindow(),  DisplayGlfw::mouseCallback_);

    glEnable(GL_DEPTH_TEST);
    glPointSize(5.0);
    glDepthFunc(GL_LESS);

    projection_ = glm::perspective(glm::radians(45.0f),
            (float)DISPLAY_GLFW_WIN_WIDTH / (float)DISPLAY_GLFW_WIN_HEIGHT,
            0.1f, 1000.0f);

	shaderMultiple_.attach(pathShaderBasic_ + "Multiple.vert");
	shaderMultiple_.attach(pathShaderBasic_ + ".frag");
	shaderMultiple_.link();

    shader_.attach(pathShaderBasic_ + ".vert");
    shader_.attach(pathShaderBasic_ + ".frag");
    shader_.link();

    std::cout << "Step : 1" << std::endl;
    snake_.setModel(pathModel_);
	std::cout << "Step : 2" << std::endl;
    block_.setModel(pathBlock_);
	std::cout << "Step : 3" << std::endl;
    ground_.setModel(pathGround_);
	std::cout << "Step : 4" << std::endl;
    wall_.setModel(pathWall_);
	std::cout << "Step : 5" << std::endl;

	testParticle_ = new Particle(ground_, winTileSize_.getX() * winTileSize_.getY());


	//asnake_.assign(&snake_);
    //ablock_ = std::make_unique< ActModel[] >(static_cast<size_t>(winTileSize_.getX() * winTileSize_.getY()));
    //int i = 0;
    //for (int y = -winTileSize_.getY() / 2; y < winTileSize_.getY() / 2; y++) { //TODO Compute vec / 2
    //    for (int x = -winTileSize_.getX() / 2; x < winTileSize_.getX() / 2; x++) {
    //        ablock_[i].assign(&block_);
    //        ablock_[i].resetTransform();
    //        ablock_[i].translate(glm::vec3(x, y, 0.f));
    //        ablock_[i].scale(glm::vec3(-0.10f));
    //        i++;
    //    }
    //}
    //asnake_.translate(glm::vec3(0.f, 0.f, 1.f));

	std::cout << "Step : 6" << std::endl;
    camera_.processPosition(Camera::Movement::BACKWARD, std::max(winTileSize_.getX(), winTileSize_.getY()) / 2);
	std::cout << "Step : 7" << std::endl;

	skybox_ = std::make_unique< Skybox >(pathShaderSkyBox_, pathDirectorySkyBox_, pathSkyBox_);
	std::cout << "Step : 8" << std::endl;
}

void                DisplayGlfw::getPath_() {

	int i = 1;
	std::cout << "Debug : " << ++i << std::endl;
    std::string pathFile = __FILE__;
	std::cout << "Debug : " << ++i << std::endl;

    std::string pathRoot = pathFile.substr(0, pathFile.rfind(DISPLAY_GLFW_SLASH));
    pathRoot_ = pathRoot.substr(0, pathRoot.rfind(DISPLAY_GLFW_SLASH));
    std::cout << pathRoot_ << std::endl;
	std::cout << "Debug : " << ++i << std::endl;

    std::ifstream t(pathRoot_ + DISPLAY_GLFW_SLASH + "file.txt");
	std::cout << "Debug : " << ++i << std::endl;
	pathModel_ = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	std::cout << "Debug : " << ++i << std::endl;
    pathBlock_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "objects" + DISPLAY_GLFW_SLASH + "nanosuit" + DISPLAY_GLFW_SLASH + "nanosuit.obj");
	std::cout << "Debug : " << ++i << std::endl;
    pathGround_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "grass_test.obj");
	std::cout << "Debug : " << ++i << std::endl;
    pathWall_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "wall.obj");
	std::cout << "Debug : " << ++i << std::endl;
	pathShaderBasic_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "shader" + DISPLAY_GLFW_SLASH + "basic");
	std::cout << "Debug : " << ++i << std::endl;
	pathShaderSkyBox_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "shader" + DISPLAY_GLFW_SLASH + "skybox");
	std::cout << "Debug 11: " << ++i << std::endl;
	pathDirectorySkyBox_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "ame_nebula" + DISPLAY_GLFW_SLASH);
	std::cout << "Debug 12: " << ++i << std::endl;
	pathSkyBox_.emplace_back("purplenebula_rt.tga");
	std::cout << "Debug 13: " << ++i << std::endl;
	pathSkyBox_.emplace_back("purplenebula_lf.tga");
	std::cout << "Debug 14: " << ++i << std::endl;
	pathSkyBox_.emplace_back("purplenebula_up.tga");
	std::cout << "Debug  15: " << ++i << std::endl;
	pathSkyBox_.emplace_back("purplenebula_dn.tga");
	std::cout << "Debug  16: " << ++i << std::endl;
	pathSkyBox_.emplace_back("purplenebula_ft.tga");
	std::cout << "Debug : 17 : " << ++i << std::endl;
	pathSkyBox_.emplace_back("purplenebula_bk.tga");
	std::cout << "Debug : " << ++i << std::endl;
}


DisplayGlfw::~DisplayGlfw() {
    clean_();
}

void DisplayGlfw::error_(std::string const &s) {
    clean_();
    throw (DisplayGlfw::GlfwConstructorException(s));
}

void DisplayGlfw::clean_() {
    //_win.close();
}

void		DisplayGlfw::setBackground(Grid< eSprite > const &grid) {
    tileBackground_ = grid;
	//actModelBackground_.resize(winTileSize_.getX() * winTileSize_.getY());
	for (int y = 0; winTileSize_.getY() > y; ++y) {
        for (int x = 0; x < winTileSize_.getX(); ++x) {
        	/*
            if (tileBackground_(x, y) == eSprite::WALL) {
                background_(x, y).assign(&wall_);
				background_(x, y).resetTransform();
				background_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 0.f));
				background_(x, y).scale(glm::vec3(-0.10f));
            }
            else if (tileBackground_(x, y) == eSprite::GROUND) {
                background_(x, y).assign(&ground_);
				background_(x, y).resetTransform();
				background_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 0.f));
				background_(x, y).scale(glm::vec3(-0.10f));
            }
*/
			testParticle_->transforms[y * winTileSize_.getX() + x].resetTransform();
			testParticle_->transforms[y * winTileSize_.getX() + x].translate(glm::vec3(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 0.f)));
			testParticle_->transforms[y * winTileSize_.getX() + x].scale(glm::vec3(-0.10f));

        }
    }
	testParticle_->update();
}

void		DisplayGlfw::drawGridCase_(eSprite sprite, int x, int y) {
	grid_(x, y).resetTransform();
	grid_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 1.2f));
	model_ = grid_(x, y).getTransform();
	shader_.setMat4("model", model_);
	grid_(x, y).getModel()->render(shader_);
}

void		DisplayGlfw::drawGrid(Grid< eSprite > const &grid) {
	tileGrid_ = grid;
	shader_.activate();

	shader_.setMat4("projection", projection_);
	shader_.setMat4("view", view_);

	for (int y = 0; y < winTileSize_.getY(); ++y) {
		for (int x = 0; x < winTileSize_.getX(); ++x) {
			if (grid(x, y) == eSprite::FOOD)
				grid_(x, y).assign(&snake_);
			else if (grid(x, y) != eSprite::HEAD)
				grid_(x, y).assign(&block_);

			if (grid(x, y) != eSprite::NONE)
				drawGridCase_(grid(x, y), x, y);
		}
	}
}

void		DisplayGlfw::interpolateGrid_() {
	for (int y = 0; y < winTileSize_.getY(); ++y) {
		for (int x = 0; x < winTileSize_.getX(); ++x) {
			eSprite sprite = tileGrid_(x, y);

			if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD
				|| (sprite & eSprite::MASK_BODY) == eSprite::BODY
				|| (sprite & eSprite::MASK_BODY) == eSprite::TAIL) {

				eSprite from = (sprite & eSprite::MASK_FROM) >> eSprite::BITWISE_FROM;
				eSprite to = (sprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;

				float distInterpelated = 1.f * (currentTimer_ / maxTimer_);

				if (to == eSprite::EAST)
					grid_(x, y).translate(glm::vec3(distInterpelated, 0.0f, 0.0f));
				else if (to == eSprite::WEST)
					grid_(x, y).translate(glm::vec3(-distInterpelated, 0.0f, 0.0f));
				else if (to == eSprite::SOUTH)
					grid_(x, y).translate(glm::vec3(0.0f, -distInterpelated, 0.0f));
				else if (to == eSprite::NORTH)
					grid_(x, y).translate(glm::vec3(0.0f, distInterpelated, 0.0f));
			}

		}
	}
}

void DisplayGlfw::render(float currentDelayFrame, float maxDelayFrame) {
	currentTimer_ = currentDelayFrame;
	maxTimer_ = maxDelayFrame;

	interpolateGrid_();

    shader_.activate();

    if (glfwGetKey(getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
    	direction_ = SOUTH;
    if (glfwGetKey(getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
    	direction_ = NORTH;
    if (glfwGetKey(getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
    	direction_ = WEST;
    if (glfwGetKey(getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
    	direction_ = EAST;
	if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		camera_.processPosition(Camera::Movement::RIGHT, deltaTime_);
	if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		camera_.processPosition(Camera::Movement::LEFT, deltaTime_);
	if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		camera_.processPosition(Camera::Movement::BACKWARD, deltaTime_);
	if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		camera_.processPosition(Camera::Movement::FORWARD, deltaTime_);

	if (glfwGetKey(getWindow(), GLFW_KEY_F) == GLFW_PRESS) {
    }

    /*
    if (glfwGetKey(getWindow(), GLFW_KEY_I) == GLFW_PRESS)
        asnake_.translate(glm::vec3(0.f, 0.f, 1.f) , deltaTime_);
    if (glfwGetKey(getWindow(), GLFW_KEY_K) == GLFW_PRESS)
        asnake_.translate(glm::vec3(0.f, 0.f, -1.f) , deltaTime_);
    if (glfwGetKey(getWindow(), GLFW_KEY_J) == GLFW_PRESS)
        asnake_.translate(glm::vec3(1.f, 0.f, 0.f) , deltaTime_);
    if (glfwGetKey(getWindow(), GLFW_KEY_L) == GLFW_PRESS)
        asnake_.translate(glm::vec3(-1.f, 0.f, 0.f) , deltaTime_);

    if (glfwGetKey(getWindow(), GLFW_KEY_T) == GLFW_PRESS)
        asnake_.rotate(glm::vec3(1.f, 0.f, 0.f), deltaTime_);
    if (glfwGetKey(getWindow(), GLFW_KEY_G) == GLFW_PRESS)
        asnake_.rotate(glm::vec3(0.f, 1.f, 0.f), deltaTime_);
    if (glfwGetKey(getWindow(), GLFW_KEY_B) == GLFW_PRESS)
        asnake_.rotate(glm::vec3(0.f, 0.f, 1.f), deltaTime_);
    */

	glDepthFunc(GL_LESS);

	view_ = camera_.getViewMatrix();
	shader_.setMat4("projection", projection_);
	shader_.setMat4("view", view_);
	/*
    model_ = asnake_.getTransform();
    shader_.setMat4("model", model_);

    snake_.render(shader_);
    */

/*
    for (int y = 0; y < winTileSize_.getY(); y++) {
        for (int x = 0; x < winTileSize_.getX(); x++) {
        	if (background_(x, y).getModel()) {
				model_ = background_(x, y).getTransform();
				shader_.setMat4("model", model_);
				background_(x, y).getModel()->render(shader_);
        	}
        }
    }
*/
	shaderMultiple_.activate();
	shaderMultiple_.setMat4("projection", projection_);
	shaderMultiple_.setMat4("view", view_);
	testParticle_->update();
	testParticle_->render(shaderMultiple_);
	//actModelBackground_.render(shaderMultiple_);

    skybox_->render(view_, projection_);

    Glfw::render();
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	shader_.activate();
}

bool        DisplayGlfw::exit() const {
	Glfw::exit();
}

void DisplayGlfw::update(float deltaTime) {
    deltaTime_ = deltaTime;
	Glfw::update();
	//if (DisplayGlfw::mouseCallbackCalled_) {
	//	camera_.processMouseMovement(DisplayGlfw::offsetX_, DisplayGlfw::offsetY_);
	//	DisplayGlfw::mouseCallbackCalled_ = false;
	//}
}

eDirection DisplayGlfw::getDirection() const {
    return (direction_);
}

DisplayGlfw::GlfwConstructorException::~GlfwConstructorException() noexcept = default;

DisplayGlfw::GlfwConstructorException::GlfwConstructorException() noexcept :
        error_("Error on Glfw constructor") { }

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(
        std::string s) noexcept :
        error_(s) {}

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(
        DisplayGlfw::GlfwConstructorException const &src) noexcept :
        error_(src.error_) { error_ = src.error_; }

const char *
DisplayGlfw::GlfwConstructorException::what() const noexcept { return (error_.c_str()); }


bool        DisplayGlfw::firstMouse_ = true;
bool        DisplayGlfw::mouseCallbackCalled_ = false;
float       DisplayGlfw::lastX_ = DISPLAY_GLFW_WIN_WIDTH / 2.0f;
float       DisplayGlfw::lastY_ = DISPLAY_GLFW_WIN_HEIGHT / 2.0f;
float       DisplayGlfw::offsetX_ = 0.f;
float       DisplayGlfw::offsetY_ = 0.f;

void DisplayGlfw::mouseCallback_(GLFWwindow* window, double xpos, double ypos) {
    if (DisplayGlfw::firstMouse_) {
        DisplayGlfw::lastX_ = static_cast<float>(xpos);
        DisplayGlfw::lastY_ = static_cast<float>(ypos);
        DisplayGlfw::firstMouse_ = false;
    }

    DisplayGlfw::offsetX_ = static_cast<float>(xpos - DisplayGlfw::lastX_);
    DisplayGlfw::offsetY_ = static_cast<float>(DisplayGlfw::lastY_ - ypos);

    DisplayGlfw::lastX_ = static_cast<float>(xpos);
    DisplayGlfw::lastY_ = static_cast<float>(ypos);
	mouseCallbackCalled_ = true;
}