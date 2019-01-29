#include <fstream>
#include <algorithm>
#include <memory>
#include <stb_image.h>
#include "DisplayGlfw.hpp"
#include "nibbler.hpp"
#include "Particle.hpp"
#include <algorithm>
#include <Carbon.framework/Headers/Carbon.h>

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
pathRoot_(NIBBLER_ROOT_PROJECT_PATH),
Glfw(windowName, DISPLAY_GLFW_WIN_WIDTH, DISPLAY_GLFW_WIN_HEIGHT),
direction_(kNorth),
currentTimer_(0.f),
maxTimer_(0.f),
winTileSize_(Vector2D<int>(width, height)),
tileBackground_(winTileSize_.getX(), winTileSize_.getY()),
background_(winTileSize_.getX(), winTileSize_.getY()),
tileGrid_(winTileSize_.getX(), winTileSize_.getY()),
grid_(winTileSize_.getX(), winTileSize_.getY()),
deltaTime_(0.016f),
particuleBackground_(nullptr),
particuleBackgroundOutline_(nullptr),
projection_(1.f),
view_(1.f),
model_(1.f),
light_(glm::vec3(0.f, 0.f, 30.f)) {

	constructMaterialMap_();
    glfwSetCursorPosCallback(getWindow(),  DisplayGlfw::mouseCallback_);

    glEnable(GL_DEPTH_TEST);
    glPointSize(5.0f);
	glLineWidth(5.0f);
    glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	projection_ = glm::perspective(glm::radians(45.0f),
            (float)DISPLAY_GLFW_WIN_WIDTH / (float)DISPLAY_GLFW_WIN_HEIGHT,
            NEAR_PLANE, MAX_PLANE);

	shaderMultiple_.attach((pathRoot_ / "dynamicLibrariesSources" / "display_glfw" / "shader" / "basicMultiple.vert").generic_string());
	shaderMultiple_.attach((pathRoot_ / "dynamicLibrariesSources" / "display_glfw" / "shader" / "basic.frag").generic_string());
	shaderMultiple_.link();

	shader_.attach((pathRoot_ / "dynamicLibrariesSources" / "display_glfw" / "shader" / "basic.vert").generic_string());
    shader_.attach((pathRoot_ / "dynamicLibrariesSources" / "display_glfw" / "shader" / "basic.frag").generic_string());
    shader_.link();

    block_.setModel((pathRoot_ / "ressources" / "objects" / "nanosuit" / "nanosuit.obj").generic_string());
    modelGrass_.setModel((pathRoot_ / "ressources" / "objects" / "grass" / "grass.obj").generic_string());
	modelSphere_.setModel((pathRoot_ / "ressources" / "objects" / "sphere.obj").generic_string());
	modelHead_.setModel((pathRoot_ / "ressources" / "objects" / "head.obj").generic_string());
    modelWall_.setModel((pathRoot_ / "ressources" / "wall.obj").generic_string());
	appleModel_.setModel((pathRoot_ / "ressources" / "Apple_obj" / "apple.obj").generic_string());

	if (winTileSize_.getY() < winTileSize_.getX())
    	camera_.processPosition(Camera::Movement::BACKWARD, winTileSize_.getX() / 2);
	else
		camera_.processPosition(Camera::Movement::BACKWARD, winTileSize_.getY() / 2);

	particuleBackground_ = new Particle((pathRoot_ / "ressources" / "objects" / "grass" / "grass.obj").generic_string(), winTileSize_.getY() * winTileSize_.getX());
	particuleBackgroundOutline_ = new Particle((pathRoot_ / "ressources" / "objects" / "grass" / "grass.obj").generic_string(), winTileSize_.getY() * winTileSize_.getX());

}
void				DisplayGlfw::constructMaterialMap_() {
	materialMap_.try_emplace(eSprite::GREEN, "GREEN");
	materialMap_.try_emplace(eSprite::GROUND, "GROUND", 0.f,
			glm::vec3(0.0756f * 0.f, 0.4423f * 0.1f, 0.07568f * 0.1f),
			glm::vec3(0.0756f, 0.4423f, 0.07568f),
			glm::vec3(0.f, 0.f, 0.f));


	materialMap_.try_emplace(eSprite::BLUE, "BLUE", 31.f,
			 glm::vec3(0.0f, 0.1f, 0.06f),
			 glm::vec3(0.0f, 0.50980392f, 0.50980392f),
			 glm::vec3(0.50196078f, 0.50196078f, 0.50196078f));
	glm::vec3 purple((213.f / 255.f), 0.f, (249.f / 255));
	materialMap_.try_emplace(eSprite::PURPLE, "PURPLE", 50.f, purple * 0.1f, purple, purple);
	glm::vec3 pink((255.f / 255.f), (64.f / 255.f), (129.f / 255));
	materialMap_.try_emplace(eSprite::PINK, "PINK", 50.f, pink * 0.1f, pink, pink);
	materialMap_.try_emplace(eSprite::GREY, "GREY", 89.6,
			 glm::vec3(0.23125f, 0.23125f, 0.23125f),
			 glm::vec3(0.2775f, 0.2775f, 0.2775f),
			 glm::vec3(0.773911f, 0.773911f, 0.773911f));
	materialMap_.try_emplace(eSprite::YELLOW, "YELLOW", 50.6f,
			glm::vec3(0.24725f, 0.1995f, 0.0745f),
			glm::vec3(0.75164f, 0.60648f, 0.22648f),
			glm::vec3(0.628281f, 0.555802f, 0.366065f));
	glm::vec3 orange((244.f / 255.f), (81.f / 255.f), (30.f / 255));
	materialMap_.try_emplace(eSprite::ORANGE, "ORANGE", 50.f, orange * 0.1f, orange, orange);
	materialMap_.try_emplace(eSprite::RED, "RED", 31.f,
			 glm::vec3(0.0f, 0.0f, 0.0f),
			 glm::vec3(0.5f, 0.0f, 0.0f),
			 glm::vec3(0.7f, 0.6f, 0.6f));
	materialMap_.try_emplace(eSprite::FOOD, "FOOD", 76.8f,
			glm::vec3(0.1745f, 0.01175f, 0.01175f),
			glm::vec3(0.61424f, 0.04136f, 0.04136f),
			glm::vec3(0.727811f, 0.626959f, 0.626959f));
	materialMap_.try_emplace(eSprite::WALL, "WALL", 0.f,
							 glm::vec3(0.05f, 0.05f, 0.05f),
							 glm::vec3(0.2f, 0.2f, 0.2f),
							 glm::vec3(0.2f, 0.2f, 0.2f));
	materialMap_.try_emplace(eSprite::NONE, "NONE", 0.f,
							 glm::vec3(0.0f),
							 glm::vec3(0.0f),
							 glm::vec3(0.0f));
}

DisplayGlfw::~DisplayGlfw() {
    clean_();
}

void DisplayGlfw::registerCallbackAction(std::function<void(eAction)>) {

}

void DisplayGlfw::error_(std::string const &s) {
    clean_();
    throw (DisplayGlfw::GlfwConstructorException(s));
}

void DisplayGlfw::clean_() {
    //_win.close();
    if (particuleBackground_)
    	delete particuleBackground_;
	if (particuleBackgroundOutline_)
		delete particuleBackgroundOutline_;
}

void		DisplayGlfw::setBackground(MutantGrid< eSprite > const &grid) {
	tileBackground_ = grid;

	for (int y = 0; winTileSize_.getY() > y; ++y) {
		for (int x = 0; x < winTileSize_.getX(); ++x) {

			particuleBackground_->transforms[y * winTileSize_.getX() + x].resetTransform();
			particuleBackground_->transforms[y * winTileSize_.getX() + x].translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 0.f));
			particuleBackground_->transforms[y * winTileSize_.getX() + x].scale(glm::vec3(-0.10f));
			particuleBackground_->transforms[y * winTileSize_.getX() + x].updateTransform();

			particuleBackgroundOutline_->transforms[y * winTileSize_.getX() + x].resetTransform();
			particuleBackgroundOutline_->transforms[y * winTileSize_.getX() + x].translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 0.f));
			particuleBackgroundOutline_->transforms[y * winTileSize_.getX() + x].scale(glm::vec3(-0.05f));
			particuleBackgroundOutline_->transforms[y * winTileSize_.getX() + x].updateTransform();
		}
	}

	particuleBackground_->update();
	particuleBackgroundOutline_->update();

}

void		DisplayGlfw::drawGridCaseBody_(int x, int y) {
	eSprite sprite = tileGrid_(x, y);

	if ((sprite & eSprite::MASK_BODY) == eSprite::TAIL)
		grid_(x, y).scale(glm::vec3(-0.1f));
	else if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD) {
		eSprite to = (sprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;

		ActModel eyeLeft = grid_(x, y);
		ActModel eyeRight = grid_(x, y);

		if (to == eSprite::EAST) {
			eyeLeft.translate(glm::vec3(0.30f, 0.15f, 0.05f));
			eyeRight.translate(glm::vec3(0.30f, -0.15f, 0.05f));
		}
		else if (to == eSprite::WEST) {
			eyeLeft.translate(glm::vec3(-0.30f, 0.15f, 0.05f));
			eyeRight.translate(glm::vec3(-0.30f, -0.15f, 0.05f));
		}
		else if (to == eSprite::SOUTH) {
			eyeLeft.translate(glm::vec3(0.15f, 0.3f, 0.05f));
			eyeRight.translate(glm::vec3(-0.15f, 0.3f, 0.05f));
		}
		else if (to == eSprite::NORTH) {
			eyeLeft.translate(glm::vec3(0.15f, -0.3f, 0.05f));
			eyeRight.translate(glm::vec3(-0.15f, -0.3f, 0.05f));
		}
		eyeLeft.scale(glm::vec3(-0.8f));
		eyeRight.scale(glm::vec3(-0.8f));

		Material::unsetMaterial(shader_);
		eyeLeft.render(shader_);
		eyeRight.render(shader_);
		materialMap_.at(sprite & eSprite::MASK_COLOR).putMaterialToShader(shader_);
	}
}

void		DisplayGlfw::drawGridCase_(eSprite sprite, int x, int y) {
	grid_(x, y).resetTransform();
	grid_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y - winTileSize_.getY() / 2, 1.f));
	drawGridCaseBody_(x, y);
	//interpolateGridCase_(x, y);
	if (static_cast<int>(sprite & eSprite::MASK_BODY) != 0) {
		grid_(x, y).scale(glm::vec3(-0.25f));
	}
	grid_(x, y).render(shader_);
}

void		DisplayGlfw::drawGrid(MutantGrid< eSprite > const &grid) {
	tileGrid_ = grid;
	shader_.activate();
	light_.putLightToShader(shader_);

	shader_.setMat4("projection", projection_);
	shader_.setMat4("view", view_);
	shader_.setVec3("uCameraPosition", camera_.getPosition());


	for (int y = 0; y < winTileSize_.getY(); ++y) {
		for (int x = 0; x < winTileSize_.getX(); ++x) {
			if ((grid(x, y) & eSprite::FOOD) == eSprite::FOOD) {
				materialMap_.at(eSprite::FOOD).putMaterialToShader(shader_);
				grid_(x, y).assign(&appleModel_);
			}
			else if (static_cast<int>(grid(x, y) & eSprite::MASK_BODY) != 0) {
				//if ((grid(x, y) & eSprite::MASK_BODY) == eSprite::HEAD)
				//	grid_(x, y).assign(&modelHead_);
				//else
					grid_(x, y).assign(&modelSphere_);
				materialMap_.at(grid(x, y) & eSprite::MASK_COLOR).putMaterialToShader(shader_);
			}
			else if ((grid(x, y) & eSprite::WALL) == eSprite::WALL) {
				grid_(x, y).assign(&modelWall_);
				materialMap_.at(eSprite::WALL).putMaterialToShader(shader_);
			}
			else if (grid(x, y) != eSprite::NONE)
				grid_(x, y).assign(&block_);

			if (grid(x, y) != eSprite::NONE)
				drawGridCase_(grid(x, y), x, y);

			if ((grid(x, y) & eSprite::WALL) == eSprite::WALL)
				renderLine_(grid_(x, y));
		}
	}
}

void		DisplayGlfw::interpolateGridCase_(int x, int y) {
	eSprite sprite = tileGrid_(x, y);

	if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD
		|| (sprite & eSprite::MASK_BODY) == eSprite::BODY
		|| (sprite & eSprite::MASK_BODY) == eSprite::TAIL) {
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

void DisplayGlfw::renderLine_(ActModel const &model) {

	ActModel copy = model;

	Material::unsetMaterial(shader_);
	copy.scale(glm::vec3(0.05f));
	copy.render(shader_, GL_LINE_STRIP);
}

void DisplayGlfw::render(float currentDelayFrame, float maxDelayFrame) {
	currentTimer_ = currentDelayFrame;
	maxTimer_ = maxDelayFrame;

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

	shader_.activate();
	shader_.setInt("uBackground", 0);
	light_.putLightToShader(shader_);
	view_ = camera_.getViewMatrix();
	shader_.setMat4("projection", projection_);
	shader_.setMat4("view", view_);

	shader_.setVec3("uCameraPosition", camera_.getPosition());
/*
    for (int y = 0; y < winTileSize_.getY(); y++) {
        for (int x = 0; x < winTileSize_.getX(); x++) {
        	if (background_(x, y).getModel()) {
				if ((tileBackground_(x, y) & eSprite::WALL) == eSprite::WALL) {
					materialMap_.at(eSprite::WALL).putMaterialToShader(shader_);
					background_(x, y).translate(glm::vec3(0.f, 0.f, 1.f));
					background_(x, y).render(shader_);
					renderLine_(background_(x, y));
					background_(x, y).translate(glm::vec3(0.f, 0.f, -1.f));
					materialMap_.at(eSprite::WALL).putMaterialToShader(shader_);
				}
				else if (tileBackground_(x, y) == eSprite::GROUND)
				 	materialMap_.at(eSprite::GROUND).putMaterialToShader(shader_);
				background_(x, y).render(shader_);
				renderLine_(background_(x, y));
        	}
        }
    }
*/

	shaderMultiple_.activate();
	light_.putLightToShader(shaderMultiple_);
	shaderMultiple_.setInt("uBackground", 1);
	shaderMultiple_.setVec3("uCameraPosition", camera_.getPosition());
	shaderMultiple_.setMat4("projection", projection_);
	shaderMultiple_.setMat4("view", view_);


	materialMap_.at(eSprite::GROUND).putMaterialToShader(shaderMultiple_);
	particuleBackground_->update();
	particuleBackground_->render(shaderMultiple_);

	Material::unsetMaterial(shaderMultiple_);
	particuleBackgroundOutline_->update();
	particuleBackgroundOutline_->render(shaderMultiple_, GL_LINE_STRIP);


    Glfw::render();
	glClearColor(0.29f * 0.35f, 0.0f, 0.51f * 0.35f, 0.40f);
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