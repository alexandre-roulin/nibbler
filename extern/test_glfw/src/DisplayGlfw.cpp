#include <nibbler.hpp>
#include "DisplayGlfw.hpp"
#include <fstream>
#include <algorithm>
#include <memory>

IDisplay *newDisplay(char const *tileset,
                     int tileSize,
                     int width,
                     int height,
                     char const *windowName) {
    return (new DisplayGlfw(tileset, tileSize, width, height, windowName));
}

void deleteDisplay(IDisplay *display) {
    delete display;
}

DisplayGlfw::DisplayGlfw(char const *tileset,
                         int tileSize,
                         int width,
                         int height,
                         char const *windowName) :
Glfw(windowName, DISPLAY_GLFW_WIN_WIDTH, DISPLAY_GLFW_WIN_HEIGHT),
direction_(NORTH),
tileSize_(tileSize),
winTileSize_(Vector2D<int>(width, height)),
winPixelSize_(Vector2D<int>(width * tileSize, height * tileSize)),
background_(winTileSize_.getX(), winTileSize_.getY()),
tileBackground_(winTileSize_.getX(), winTileSize_.getY()),
deltaTime_(0.016f),
projection_(1.f),
view_(1.f),
model_(1.f) {

    getPath_();

    glfwSetCursorPosCallback(getWindow(),  DisplayGlfw::mouseCallback_);


    glEnable(GL_DEPTH_TEST);
    glPointSize(5.0);
    glDepthFunc(GL_LESS);

    projection_ = glm::perspective(glm::radians(45.0f),
            (float)DISPLAY_GLFW_WIN_WIDTH / (float)DISPLAY_GLFW_WIN_HEIGHT,
            0.1f, 1000.0f);

    shader_.attach(pathShaderVert_);
    shader_.attach(pathShaderFrag_);
    shader_.link();

    snake_.setModel(pathModel_);
    block_.setModel(pathBlock_);
    ground_.setModel(pathGround_);
    wall_.setModel(pathWall_);

    asnake_.assign(&snake_);
    ablock_ = std::make_unique< ActModel[] >(winTileSize_.getX() * winTileSize_.getY());
    int i = 0;
    for (int y = -winTileSize_.getY() / 2; y < winTileSize_.getY() / 2; y++) { //TODO Compute vec / 2
        for (int x = -winTileSize_.getX() / 2; x < winTileSize_.getX() / 2; x++) {
            ablock_[i].assign(&block_);
            ablock_[i].resetTransform();
            ablock_[i].translate(glm::vec3(x, y, 0.f));
            ablock_[i].scale(glm::vec3(-0.10f));
            i++;
        }
    }

    asnake_.translate(glm::vec3(0.f, 0.f, 1.f));

    camera_.processPosition(Camera::Movement::BACKWARD, std::max(winTileSize_.getX(), winTileSize_.getY()) / 2);



}

void                DisplayGlfw::getPath_() {

    std::string pathFile = __FILE__;

    std::string pathRoot = pathFile.substr(0, pathFile.rfind(DISPLAY_GLFW_SLASH));
    pathRoot_ = pathRoot.substr(0, pathRoot.rfind(DISPLAY_GLFW_SLASH));
    std::cout << pathRoot_ << std::endl;

    std::ifstream t(pathRoot_ + DISPLAY_GLFW_SLASH + "file.txt");
    pathModel_ = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    pathBlock_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "objects" + DISPLAY_GLFW_SLASH + "nanosuit" + DISPLAY_GLFW_SLASH + "nanosuit.obj");
    pathGround_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "try.obj");
    pathWall_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "resources" + DISPLAY_GLFW_SLASH + "wall.obj");
    pathShaderVert_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "shader" + DISPLAY_GLFW_SLASH + "basic.vert");
    pathShaderFrag_ = std::string(pathRoot_ + DISPLAY_GLFW_SLASH + "shader" + DISPLAY_GLFW_SLASH + "basic.frag");
}


DisplayGlfw::~DisplayGlfw(void) {
    clean_();
}

void DisplayGlfw::error_(std::string const &s) {
    clean_();
    throw (DisplayGlfw::GlfwConstructorException(s));
}

void DisplayGlfw::clean_() {
    //_win.close();
}

void		DisplayGlfw::setBackground(Grid<int> const &grid) {
    tileBackground_ = grid;
    for (int y = 0; y < winTileSize_.getY(); ++y) {
        for (int x = 0; x < winTileSize_.getX(); ++x) {
            if (tileBackground_(x, y) == SPRITE_WALL) {
                background_(x, y).assign(&wall_);
				background_(x, y).resetTransform();
				background_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 0.f));
				background_(x, y).scale(glm::vec3(-0.10f));
            }
            else if (tileBackground_(x, y) == SPRITE_GROUND) {
                background_(x, y).assign(&ground_);
				background_(x, y).resetTransform();
				background_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 0.f));
				background_(x, y).scale(glm::vec3(-0.10f));
            }
        }
    }
}

void		DisplayGlfw::drawGrid(Grid<int> const &grid) {
	shader_.activate();

	shader_.setMat4("projection", projection_);
	shader_.setMat4("view", view_);

	for (int y = 0; y < winTileSize_.getY(); ++y) {
		for (int x = 0; x < winTileSize_.getX(); ++x) {
			if (grid(x, y) == SPRITE_FOOD) {
				asnake_.resetTransform();
				asnake_.translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 1.2f));
				model_ = asnake_.getTransform();
				shader_.setMat4("model", model_);
				asnake_.getModel()->render(shader_);

			}
			else if (grid(x, y) != -1) {
				ablock_[0].resetTransform();
				ablock_[0].translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 1.2f));
				model_ = ablock_[0].getTransform();
				shader_.setMat4("model", model_);
				ablock_[0].getModel()->render(shader_);
			}
		}
	}
}


void DisplayGlfw::render(void) {
    shader_.activate();

    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS)
    	direction_ = SOUTH;
        //camera_.processPosition(Camera::Movement::FORWARD, deltaTime_);
    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS)
    	direction_ = NORTH;
        //camera_.processPosition(Camera::Movement::BACKWARD, deltaTime_);
    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS)
    	direction_ = WEST;
        //camera_.processPosition(Camera::Movement::LEFT, deltaTime_);
    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS)
    	direction_ = EAST;
        //camera_.processPosition(Camera::Movement::RIGHT, deltaTime_);
    if (glfwGetKey(getWindow(), GLFW_KEY_F) == GLFW_PRESS) {
    }

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

    view_ = camera_.getViewMatrix();
    model_ = asnake_.getTransform();
    shader_.setMat4("projection", projection_);
    shader_.setMat4("view", view_);
    shader_.setMat4("model", model_);

    snake_.render(shader_);

    /*
    for (int i = 0; i < winTileSize_.getX() * winTileSize_.getY(); i++) {
        model_ = ablock_[i].getTransform();
        shader_.setMat4("model", model_);
		ablock_[i].getModel()->render(shader_);
    }
    */
    for (int y = 0; y < winTileSize_.getY(); y++) {
        for (int x = 0; x < winTileSize_.getX(); x++) {
        	if (background_(x, y).getModel()) {
				model_ = background_(x, y).getTransform();
				shader_.setMat4("model", model_);
				background_(x, y).getModel()->render(shader_);

        	}
        }
    }
    Glfw::render();
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

bool        DisplayGlfw::exit(void) const {
	Glfw::exit();
}

/*
**####################ID_TILE
*/

void DisplayGlfw::update(float deltaTime) {
    deltaTime_ = deltaTime;
    Glfw::update();
    if (DisplayGlfw::mouseCallbackCalled_) {
        camera_.processMouseMovement(DisplayGlfw::offsetX_, DisplayGlfw::offsetY_);
        DisplayGlfw::mouseCallbackCalled_ = false;
    }
}
void DisplayGlfw::update() {
	Glfw::update();
	if (DisplayGlfw::mouseCallbackCalled_) {
		camera_.processMouseMovement(DisplayGlfw::offsetX_, DisplayGlfw::offsetY_);
		DisplayGlfw::mouseCallbackCalled_ = false;
	}
}

void        DisplayGlfw::setFrameTime(float frameTime) {
    frameTime_ = frameTime;
}


eDirection DisplayGlfw::getDirection(void) const {
    return (direction_);
}

DisplayGlfw::GlfwConstructorException::~GlfwConstructorException(
        void) throw() {}

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(void) throw() :
        error_("Error on Glfw constructor") {}

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(
        std::string s) throw() :
        error_(s) {}

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(
        DisplayGlfw::GlfwConstructorException const &src) throw() :
        error_(src.error_) { error_ = src.error_; }

const char *
DisplayGlfw::GlfwConstructorException::what() const throw() { return (error_.c_str()); }


bool        DisplayGlfw::firstMouse_ = true;
bool        DisplayGlfw::mouseCallbackCalled_ = false;
float       DisplayGlfw::lastX_ = DISPLAY_GLFW_WIN_WIDTH / 2.0f;
float       DisplayGlfw::lastY_ = DISPLAY_GLFW_WIN_HEIGHT / 2.0f;
float       DisplayGlfw::offsetX_ = 0.f;
float       DisplayGlfw::offsetY_ = 0.f;

void DisplayGlfw::mouseCallback_(GLFWwindow* window, double xpos, double ypos) {
    if (DisplayGlfw::firstMouse_) {
        DisplayGlfw::lastX_ = xpos;
        DisplayGlfw::lastY_ = ypos;
        DisplayGlfw::firstMouse_ = false;
    }

    DisplayGlfw::offsetX_ = xpos - DisplayGlfw::lastX_;
    DisplayGlfw::offsetY_ = DisplayGlfw::lastY_ - ypos;

    DisplayGlfw::lastX_ = xpos;
    DisplayGlfw::lastY_ = ypos;
	mouseCallbackCalled_ = true;
}