#include <fstream>
#include <algorithm>
#include <memory>
#include <stb_image.h>
#include "DisplayGlfw.hpp"
#include "nibbler.hpp"
#include "Skybox.hpp"
#include <algorithm>

#define PARTICULE_SIZE 1
#define NEAR_PLANE 0.1f
#define MAX_PLANE 1000.f

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
direction_(kNorth),
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
model_(1.f),
light_(glm::vec3(0.f, 0.f, 30.f)) {

	getPath_();
	constructMaterialMap_();
    glfwSetCursorPosCallback(getWindow(),  DisplayGlfw::mouseCallback_);

    glEnable(GL_DEPTH_TEST);
    glPointSize(5.0);
    glDepthFunc(GL_LESS);

    projection_ = glm::perspective(glm::radians(45.0f),
            (float)DISPLAY_GLFW_WIN_WIDTH / (float)DISPLAY_GLFW_WIN_HEIGHT,
            NEAR_PLANE, MAX_PLANE);

	shaderMultiple_.attach(pathShaderBasic_ + "Multiple.vert");
	shaderMultiple_.attach(pathShaderBasic_ + "Multiple.frag");
	shaderMultiple_.link();

    shader_.attach(pathShaderBasic_ + ".vert");
    shader_.attach(pathShaderBasic_ + ".frag");
    shader_.link();

    snake_.setModel(pathModel_);
    block_.setModel(pathBlock_);
    modelGrass_.setModel(pathGrass_);
	modelSphere_.setModel(std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "objects" + DISPLAY_GLFW_SLASH + "sphere.obj"));
    modelWall_.setModel(pathWall_);
	appleModel_.setModel(pathAppleModel_);

	testParticle_ = new Particle(modelGrass_, PARTICULE_SIZE*PARTICULE_SIZE*PARTICULE_SIZE);
	for (int y = 0; y < PARTICULE_SIZE; ++y) {
		for (int x = 0; x < PARTICULE_SIZE; ++x) {
			for (int z = 0; z < PARTICULE_SIZE; ++z) {
				testParticle_->transforms[x + PARTICULE_SIZE * (y + PARTICULE_SIZE * z)].translate(glm::vec3(x, y, z));
				testParticle_->transforms[x + PARTICULE_SIZE * (y + PARTICULE_SIZE * z)].scale(glm::vec3(-0.10f));
			}
		}
	}

	if (winTileSize_.getX() > winTileSize_.getY())
    	camera_.processPosition(Camera::Movement::BACKWARD, winTileSize_.getX() / 2);
	else
		camera_.processPosition(Camera::Movement::BACKWARD, winTileSize_.getY() / 2);
	skybox_ = std::make_unique< Skybox >(pathShaderSkyBox_, pathDirectorySkyBox_, pathSkyBox_);
}
void				DisplayGlfw::constructMaterialMap_() {
	materialMap_.try_emplace(eSprite::GREEN, "GREEN");
	materialMap_.try_emplace(eSprite::GROUND, "GROUND", 0.f,
			glm::vec3(0.0756f * 0.f, 0.61423f * 0.1f, 0.07568f * 0.1f),
			glm::vec3(0.0756f, 0.61423f, 0.07568f),
			glm::vec3(0.f, 0.f, 0.f));


	materialMap_.try_emplace(eSprite::BLUE, "BLUE", 31.f,
			 glm::vec3(0.0f, 0.1f, 0.06f),
			 glm::vec3(0.0f, 0.50980392f, 0.50980392f),
			 glm::vec3(0.50196078f, 0.50196078f, 0.50196078f));
	materialMap_.try_emplace(eSprite::PURPLE, "PURPLE");
	materialMap_.try_emplace(eSprite::PINK, "PINK");
	materialMap_.try_emplace(eSprite::GREY, "GREY");
	materialMap_.try_emplace(eSprite::YELLOW, "YELLOW", 50.6f,
			glm::vec3(0.24725f, 0.1995f, 0.0745f),
			glm::vec3(0.75164f, 0.60648f, 0.22648f),
			glm::vec3(0.628281f, 0.555802f, 0.366065f));
	materialMap_.try_emplace(eSprite::ORANGE, "ORANGE");
	materialMap_.try_emplace(eSprite::RED, "RED", 31.f,
			 glm::vec3(0.0f, 0.0f, 0.0f),
			 glm::vec3(0.5f, 0.0f, 0.0f),
			 glm::vec3(0.7f, 0.6f, 0.6f));
	materialMap_.try_emplace(eSprite::WALL, "WALL", 0.f,
							 glm::vec3(0.1f, 0.1f, 0.1f),
							 glm::vec3(0.5f, 0.5f, 0.5f),
							 glm::vec3(0.5f, 0.5f, 0.5f));
}
void                DisplayGlfw::getPath_() {

    std::string pathFile = __FILE__;

    std::string pathRoot = pathFile.substr(0, pathFile.rfind(DISPLAY_GLFW_SLASH));
    pathRoot_ = pathRoot.substr(0, pathRoot.rfind(DISPLAY_GLFW_SLASH));
    std::cout << pathRoot_ << std::endl;
	pathModel_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "snakest2" + DISPLAY_GLFW_SLASH + "Snake_by_Swp.obj");
    pathBlock_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "objects" + DISPLAY_GLFW_SLASH + "nanosuit" + DISPLAY_GLFW_SLASH + "nanosuit.obj");
    pathGrass_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "objects" + DISPLAY_GLFW_SLASH + "grass" + DISPLAY_GLFW_SLASH + "grass.obj");
    pathWall_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "wall.obj");
	pathShaderBasic_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "shader" + DISPLAY_GLFW_SLASH + "basic");
	pathShaderSkyBox_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "shader" + DISPLAY_GLFW_SLASH + "skybox");
	pathDirectorySkyBox_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "ame_nebula" + DISPLAY_GLFW_SLASH);
	pathAppleModel_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "Apple_obj" + DISPLAY_GLFW_SLASH + "apple.obj");
	pathSkyBox_.emplace_back("purplenebula_rt.tga");
	pathSkyBox_.emplace_back("purplenebula_lf.tga");
	pathSkyBox_.emplace_back("purplenebula_up.tga");
	pathSkyBox_.emplace_back("purplenebula_dn.tga");
	pathSkyBox_.emplace_back("purplenebula_ft.tga");
	pathSkyBox_.emplace_back("purplenebula_bk.tga");
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
	for (int y = 0; winTileSize_.getY() > y; ++y) {
        for (int x = 0; x < winTileSize_.getX(); ++x) {

            if (tileBackground_(x, y) == eSprite::WALL) {
                background_(x, y).assign(&modelWall_);
				background_(x, y).resetTransform();
				background_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 0.f));
				background_(x, y).scale(glm::vec3(-0.10f));
            }
            else if (tileBackground_(x, y) == eSprite::GROUND) {
                background_(x, y).assign(&modelGrass_);
				background_(x, y).resetTransform();
				background_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 0.f));
				//background_(x, y).rotate(glm::vec3(1.f, 0.f, 0.f), glm::radians(90.f), 1.f);
				background_(x, y).scale(glm::vec3(-0.10f));
            }
        }
    }
}

void		DisplayGlfw::drawGridCase_(eSprite sprite, int x, int y) {
	grid_(x, y).resetTransform();
	grid_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 1.f));
	//interpolateGridCase_(x, y);
	if (static_cast<int>(sprite & eSprite::MASK_BODY) != 0) {
		//grid_(x, y).rotate(glm::vec3(1.f, 0.f, 0.f), glm::radians(90.f), 1.f);
		grid_(x, y).scale(glm::vec3(-0.25f));
	}
	model_ = grid_(x, y).getTransform();
	shader_.setMat4("model", model_);
	grid_(x, y).getModel()->render(shader_);
}

void		DisplayGlfw::drawGrid(Grid< eSprite > const &grid) {
	tileGrid_ = grid;
	shader_.activate();
	light_.putLightToShader(shader_);

	shader_.setMat4("projection", projection_);
	shader_.setMat4("view", view_);
	shader_.setVec3("uCameraPosition", camera_.getPosition());

	for (int y = 0; y < winTileSize_.getY(); ++y) {
		for (int x = 0; x < winTileSize_.getX(); ++x) {
			if ((grid(x, y) & eSprite::FOOD) == eSprite::FOOD) {
				grid_(x, y).assign(&appleModel_);
			}
			else if (grid(x, y) != eSprite::HEAD)
				grid_(x, y).assign(&block_);


			if (static_cast<int>(grid(x, y) & eSprite::MASK_BODY) != 0) {
				grid_(x, y).assign(&modelSphere_);
				materialMap_.at(grid(x, y) & eSprite::MASK_COLOR).putMaterialToShader(shader_);
				std::cout << "ModelSphere" << std::endl;
			}
			else if ((grid(x, y) & eSprite::FOOD) == eSprite::FOOD) {
				materialMap_.at(eSprite::RED).putMaterialToShader(shader_);
			}
			if (grid(x, y) != eSprite::NONE)
				drawGridCase_(grid(x, y), x, y);
		}
	}
}

void		DisplayGlfw::interpolateGridCase_(int x, int y) {
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
			grid_(x, y).translate(glm::vec3(0.0f, distInterpelated, 0.0f));
		else if (to == eSprite::NORTH)
			grid_(x, y).translate(glm::vec3(0.0f, -distInterpelated, 0.0f));
	}
}

void DisplayGlfw::render(float currentDelayFrame, float maxDelayFrame) {
	currentTimer_ = currentDelayFrame;
	maxTimer_ = maxDelayFrame;

    shader_.activate();
	light_.putLightToShader(shader_);

    if (glfwGetKey(getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
    	direction_ = kSouth;
    if (glfwGetKey(getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
    	direction_ = kNorth;
    if (glfwGetKey(getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
    	direction_ = kWest;
    if (glfwGetKey(getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
    	direction_ = kEast;
	if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		camera_.processPosition(Camera::Movement::RIGHT, deltaTime_ * 5);
	if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		camera_.processPosition(Camera::Movement::LEFT, deltaTime_ * 5);
	if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		camera_.processPosition(Camera::Movement::BACKWARD, deltaTime_ * 5);
	if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		camera_.processPosition(Camera::Movement::FORWARD, deltaTime_ * 5);

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
	shader_.setInt("uBackground", 1);
	shader_.setVec3("uCameraPosition", camera_.getPosition());

    for (int y = 0; y < winTileSize_.getY(); y++) {
        for (int x = 0; x < winTileSize_.getX(); x++) {
        	if (background_(x, y).getModel()) {
				model_ = background_(x, y).getTransform();
				shader_.setMat4("model", model_);
				if (tileBackground_(x, y) == eSprite::WALL)
					materialMap_.at(eSprite::WALL).putMaterialToShader(shader_);
				else if (tileBackground_(x, y) == eSprite::GROUND)
				 	materialMap_.at(eSprite::GROUND).putMaterialToShader(shader_);
				background_(x, y).getModel()->render(shader_);
        	}
        }
    }
	shader_.setInt("uBackground", 0);

	shaderMultiple_.activate();
	shaderMultiple_.setMat4("projection", projection_);
	shaderMultiple_.setMat4("view", view_);

/*
	testParticle_->updatePhysicsMovement(attractor, deltaTime_);
	testParticle_->update();
	testParticle_->render(shaderMultiple_);
*/
    skybox_->render(view_, projection_);

    Glfw::render();
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	shader_.activate();
}

bool        DisplayGlfw::exit() const {
	return Glfw::exit();
}

void DisplayGlfw::update(float deltaTime) {
    deltaTime_ = deltaTime;
	Glfw::update();
	if (!cursor_ && DisplayGlfw::mouseCallbackCalled_) {
		camera_.processMouseMovement(DisplayGlfw::offsetX_, DisplayGlfw::offsetY_);
		DisplayGlfw::mouseCallbackCalled_ = false;
	}
}

eDirection DisplayGlfw::getDirection() const {
    return (direction_);
}

/*
std::map< eSprite, Material >		DisplayGlfw::materialMap_ = {
		{ eSprite::GREEN,  0x33a361}, //
		{ eSprite::BLUE, 0x124fd1 }, //
		{ eSprite::PURPLE, 0x4a0078 }, //
		{ eSprite::PINK, 0xcf0047 }, //
		{ eSprite::GREY, 0x8c8a8a },
		{ eSprite::YELLOW, 0xe6cf24 }, //
		{ eSprite::ORANGE, 0xf78b00 },
		{ eSprite::RED, 0xed3300 } //
};
*/

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

void DisplayGlfw::mouseCallback_(GLFWwindow *window, double xpos, double ypos) {
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