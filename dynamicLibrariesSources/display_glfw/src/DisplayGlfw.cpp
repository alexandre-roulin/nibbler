#include <fstream>
#include "DisplayGlfw.hpp"
//#include <Carbon.framework/Headers/Carbon.h>

#define NEAR_PLANE 0.1f
#define MAX_PLANE 1000.f

#define CAMERA_GLOBAL 0
#define CAMERA_SNAKE 1
#define CAMERA_MOVABLE 2

#define CAMERA_SIZE 3

IDisplay *newInstance(int width,
					 int height,
					 char const *windowName) {
	return (new DisplayGlfw(width, height, windowName));
}

void deleteInstance(IDisplay *display) {
	delete display;
}

DisplayGlfw::DisplayGlfw(int width,
						 int height,
						 char const *windowName) :
		Glfw(windowName, DISPLAY_GLFW_WIN_WIDTH, DISPLAY_GLFW_WIN_HEIGHT),
		pathRoot_(NIBBLER_ROOT_PROJECT_PATH),
		direction_(kNorth),
		winTileSize_(Vector2D<int>(width, height)),
		tileBackground_(winTileSize_.getX(), winTileSize_.getY()),
		background_(winTileSize_.getX(), winTileSize_.getY()),
		tileGrid_(winTileSize_.getX(), winTileSize_.getY()),
		grid_(winTileSize_.getX(), winTileSize_.getY()),
		deltaTime_(0.016f),
		yourSnakeSprite(eSprite::kNone),
		yourSnakeX(-10),
		yourSnakeY(-10),
		particuleBackground_(nullptr),
		particuleBackgroundOutline_(nullptr),
		light_(glm::vec3(0.f, 0.f, 30.f)),
		indexActiveCamera_(0),
		projection_(1.f),
		view_(1.f),
		model_(1.f),
		callback_(nullptr) {
	constructMaterialMap_();

	glEnable(GL_DEPTH_TEST);
	glPointSize(5.0f);
	glLineWidth(5.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	projection_ = glm::perspective(glm::radians(45.0f),
								   (float) DISPLAY_GLFW_WIN_WIDTH / (float) DISPLAY_GLFW_WIN_HEIGHT,
								   NEAR_PLANE, MAX_PLANE);

	shaderMultiple_.attach((pathRoot_ / "dynamicLibrariesSources" / "display_glfw" / "shader" /
							"basicMultiple.vert").generic_string());
	shaderMultiple_.attach(
			(pathRoot_ / "dynamicLibrariesSources" / "display_glfw" / "shader" / "basic.frag").generic_string());
	shaderMultiple_.link();

	shader_.attach((pathRoot_ / "dynamicLibrariesSources" / "display_glfw" / "shader" / "basic.vert").generic_string());
	shader_.attach((pathRoot_ / "dynamicLibrariesSources" / "display_glfw" / "shader" / "basic.frag").generic_string());
	shader_.link();

	modelGrass_.setModel((pathRoot_ / "ressources" / "objects" / "grass" / "grass.obj").generic_string());
	modelSphere_.setModel((pathRoot_ / "ressources" / "objects" / "sphere.obj").generic_string());
	modelHead_.setModel((pathRoot_ / "ressources" / "objects" / "head.obj").generic_string());
	modelWall_.setModel((pathRoot_ / "ressources" / "wall.obj").generic_string());
	appleModel_.setModel((pathRoot_ / "ressources" / "Apple_obj" / "apple.obj").generic_string());
	actBlock_.assign(&modelGrass_);


	camera_.reserve(CAMERA_SIZE);
	camera_.emplace_back();
	camera_.emplace_back();
	camera_.emplace_back();

	camera_[CAMERA_GLOBAL].setPosition(glm::vec3(-0.5f, winTileSize_.getY() * -0.9, winTileSize_.getY() * 0.8f));
	camera_[CAMERA_MOVABLE].setPosition(glm::vec3(0.f, 0.f, winTileSize_.getX() * 2.f));

	glm::vec3 centerOfGrid(0.f, camera_[CAMERA_GLOBAL].getPosition().y, camera_[CAMERA_GLOBAL].getPosition().z);
	camera_[CAMERA_GLOBAL].setFront(-centerOfGrid);
	camera_[CAMERA_GLOBAL].setUp(glm::vec3(0.f, 1.f, 0.f));

	particuleBackground_ = new Particle((pathRoot_ / "ressources" / "objects" / "grass" / "grass.obj").generic_string(),
										winTileSize_.getY() * winTileSize_.getX());
	particuleBackgroundOutline_ = new Particle(
			(pathRoot_ / "ressources" / "objects" / "grass" / "grass.obj").generic_string(),
			winTileSize_.getY() * winTileSize_.getX());

}

void DisplayGlfw::constructMaterialMap_() {
	materialMap_.try_emplace(eSprite::kGreen, "GREEN");
	materialMap_.try_emplace(eSprite::kGround, "GROUND", 0.f,
							 glm::vec3(0.0756f * 0.f, 0.4423f * 0.1f, 0.07568f * 0.1f),
							 glm::vec3(0.0756f, 0.5523f, 0.07568f),
							 glm::vec3(0.f, 0.f, 0.f));
	materialMap_.try_emplace(eSprite::kBlue, "BLUE", 31.f,
							 glm::vec3(0.0f, 0.1f, 0.06f),
							 glm::vec3(0.0f, 0.50980392f, 0.50980392f),
							 glm::vec3(0.50196078f, 0.50196078f, 0.50196078f));
	glm::vec3 purple((213.f / 255.f), 0.f, (249.f / 255));
	materialMap_.try_emplace(eSprite::kPurple, "PURPLE", 50.f, purple * 0.1f, purple, purple);
	glm::vec3 pink((255.f / 255.f), (64.f / 255.f), (129.f / 255));
	materialMap_.try_emplace(eSprite::kPink, "PINK", 50.f, pink * 0.1f, pink, pink);
	materialMap_.try_emplace(eSprite::kGrey, "GREY", 89.6,
							 glm::vec3(0.23125f, 0.23125f, 0.23125f),
							 glm::vec3(0.2775f, 0.2775f, 0.2775f),
							 glm::vec3(0.773911f, 0.773911f, 0.773911f));
	materialMap_.try_emplace(eSprite::kYellow, "YELLOW", 50.6f,
							 glm::vec3(0.19f, 0.19f, 0.0545f),
							 glm::vec3(0.85f, 0.85f, 0.1f),
							 glm::vec3(0.628281f, 0.628281f, 0.628281f));
	glm::vec3 orange((244.f / 255.f), (81.f / 255.f), (30.f / 255));
	materialMap_.try_emplace(eSprite::kOrange, "ORANGE", 50.f, orange * 0.1f, orange, orange);
	materialMap_.try_emplace(eSprite::kRed, "RED", 31.f,
							 glm::vec3(0.2f, 0.07f, 0.07f),
							 glm::vec3(0.7f, 0.07f, 0.07f),
							 glm::vec3(0.7f, 0.6f, 0.6f));
	materialMap_.try_emplace(eSprite::kFood, "FOOD", 76.8f,
							 glm::vec3(0.1745f, 0.01175f, 0.01175f),
							 glm::vec3(0.61424f, 0.04136f, 0.04136f),
							 glm::vec3(0.727811f, 0.626959f, 0.626959f));
	materialMap_.try_emplace(eSprite::kWall, "WALL", 0.f,
							 glm::vec3(0.05f, 0.05f, 0.05f),
							 glm::vec3(0.2f, 0.2f, 0.2f),
							 glm::vec3(0.2f, 0.2f, 0.2f));
	materialMap_.try_emplace(eSprite::kNone, "NONE", 0.f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
}

DisplayGlfw::~DisplayGlfw() {
	clean_();
}

void DisplayGlfw::error_(std::string const &s) {
	clean_();
	throw (DisplayGlfw::GlfwConstructorException(s));
}

void DisplayGlfw::clean_() {
	if (particuleBackground_)
		delete particuleBackground_;
	if (particuleBackgroundOutline_)
		delete particuleBackgroundOutline_;
}

void DisplayGlfw::setBackground(MutantGrid<eSprite> const &grid) {
	tileBackground_ = grid;

	for (int y = 0; winTileSize_.getY() > y; ++y) {
		for (int x = 0; x < winTileSize_.getX(); ++x) {
			particuleBackground_->transforms[y * winTileSize_.getX() + x].resetTransform();
			particuleBackground_->transforms[y * winTileSize_.getX() + x].translate(
					glm::vec3(x - winTileSize_.getX() / 2, y * -1 + winTileSize_.getY() / 2, 0.f));
			particuleBackground_->transforms[y * winTileSize_.getX() + x].scale(glm::vec3(-0.10f));
			particuleBackground_->transforms[y * winTileSize_.getX() + x].updateTransform();

			particuleBackgroundOutline_->transforms[y * winTileSize_.getX() + x].resetTransform();
			particuleBackgroundOutline_->transforms[y * winTileSize_.getX() + x].translate(
					glm::vec3(x - winTileSize_.getX() / 2, y * -1 + winTileSize_.getY() / 2, 0.f));
			particuleBackgroundOutline_->transforms[y * winTileSize_.getX() + x].scale(glm::vec3(-0.05f));
			particuleBackgroundOutline_->transforms[y * winTileSize_.getX() + x].updateTransform();
		}
	}
	particuleBackground_->update();
	particuleBackgroundOutline_->update();
}

void DisplayGlfw::drawGridCaseBody_(int x, int y) {
	eSprite sprite = tileGrid_(x, y);


	if ((sprite & eSprite::kMaskBody) == eSprite::kTail)
		grid_(x, y).scale(glm::vec3(-0.1f));
	else if ((sprite & eSprite::kMaskBody) == eSprite::kHead) {
		eSprite to = (sprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo;

		ActModel eyeLeft = grid_(x, y);
		ActModel eyeRight = grid_(x, y);

		if (to == eSprite::kEast) {
			eyeLeft.translate(glm::vec3(0.30f, 0.15f, 0.05f));
			eyeRight.translate(glm::vec3(0.30f, -0.15f, 0.05f));
		} else if (to == eSprite::kWest) {
			eyeLeft.translate(glm::vec3(-0.30f, 0.15f, 0.05f));
			eyeRight.translate(glm::vec3(-0.30f, -0.15f, 0.05f));
		} else if (to == eSprite::kNorth) {
			eyeLeft.translate(glm::vec3(0.15f, 0.3f, 0.05f));
			eyeRight.translate(glm::vec3(-0.15f, 0.3f, 0.05f));
		} else if (to == eSprite::kSouth) {
			eyeLeft.translate(glm::vec3(0.15f, -0.3f, 0.05f));
			eyeRight.translate(glm::vec3(-0.15f, -0.3f, 0.05f));
		}
		eyeLeft.scale(glm::vec3(-0.8f));
		eyeRight.scale(glm::vec3(-0.8f));

		Material::unsetMaterial(shader_);
		eyeLeft.render(shader_);
		eyeRight.render(shader_);
		materialMap_.at(sprite & eSprite::kMaskColor).putMaterialToShader(shader_);
		if ((sprite & eSprite::kYourSnake) == eSprite::kYourSnake) {
			yourSnakeSprite = sprite;
			bYourSnakeSprite_ = true;
			yourSnakeX = x;
			yourSnakeY = y;
			camera_[CAMERA_SNAKE].setPosition(grid_(x, y).getPosition() + glm::vec3(0.f, 0.f, 10.f));
			if (to == eSprite::kEast)
				camera_[CAMERA_SNAKE].setPosition(camera_[CAMERA_SNAKE].getPosition() + glm::vec3(-20.f, 0.f, 0.f));
			else if (to == eSprite::kWest)
				camera_[CAMERA_SNAKE].setPosition(camera_[CAMERA_SNAKE].getPosition() + glm::vec3(20.f, 0.f, 0.f));
			else if (to == eSprite::kNorth)
				camera_[CAMERA_SNAKE].setPosition(camera_[CAMERA_SNAKE].getPosition() + glm::vec3(0.f, -20.f, 0.f));
			else if (to == eSprite::kSouth)
				camera_[CAMERA_SNAKE].setPosition(camera_[CAMERA_SNAKE].getPosition() + glm::vec3(0.f, 20.f, 0.f));
			camera_[CAMERA_SNAKE].setFront(grid_(x, y).getPosition() - camera_[CAMERA_SNAKE].getPosition());
			camera_[CAMERA_SNAKE].setUp(glm::vec3(0.f, 0.f, 1.f));
		}
	}
}

void DisplayGlfw::drawGridCase_(eSprite sprite, int x, int y) {
	grid_(x, y).resetTransform();
	grid_(x, y).translate(glm::vec3(x - winTileSize_.getX() / 2, y * -1 + winTileSize_.getY() / 2, 1.f));
	drawGridCaseBody_(x, y);
	//interpolateGridCase_(x, y);
	if (static_cast<int>(sprite & eSprite::kMaskBody) != 0) {
		grid_(x, y).scale(glm::vec3(-0.25f));
	}
	grid_(x, y).render(shader_);
}

void DisplayGlfw::activeNextCamera_() {
	++indexActiveCamera_;
	indexActiveCamera_ = indexActiveCamera_ % camera_.size();
}

Camera &DisplayGlfw::getActiveCamera_() {
	return (camera_[indexActiveCamera_]);
}

void DisplayGlfw::drawGrid(MutantGrid<eSprite> const &grid) {
	tileGrid_ = grid;
	shader_.activate();
	light_.putLightToShader(shader_);

	shader_.setMat4("projection", projection_);
	shader_.setMat4("view", view_);
	shader_.setVec3("uCameraPosition", getActiveCamera_().getPosition());


	bYourSnakeSprite_ = false;
	for (int y = 0; y < winTileSize_.getY(); ++y) {
		for (int x = 0; x < winTileSize_.getX(); ++x) {
			if ((grid(x, y) & eSprite::kFood) == eSprite::kFood) {
				materialMap_.at(eSprite::kFood).putMaterialToShader(shader_);
				grid_(x, y).assign(&appleModel_);
			} else if (static_cast<int>(grid(x, y) & eSprite::kMaskBody) != 0) {
				grid_(x, y).assign(&modelSphere_);
				materialMap_.at(grid(x, y) & eSprite::kMaskColor).putMaterialToShader(shader_);
			} else if ((grid(x, y) & eSprite::kWall) == eSprite::kWall) {
				grid_(x, y).assign(&modelWall_);
				materialMap_.at(eSprite::kWall).putMaterialToShader(shader_);
			} else if (grid(x, y) != eSprite::kNone)
				grid_(x, y).assign(&modelWall_);

			if (grid(x, y) != eSprite::kNone)
				drawGridCase_(grid(x, y), x, y);

			if ((grid(x, y) & eSprite::kWall) == eSprite::kWall)
				renderLine_(grid_(x, y));
		}
	}
	if (!bYourSnakeSprite_)
		yourSnakeSprite = eSprite::kNone;
}

void DisplayGlfw::renderLine_(ActModel const &model) {

	ActModel copy = model;

	Material::unsetMaterial(shader_);
	copy.scale(glm::vec3(0.05f));
	copy.render(shader_, GL_LINE_STRIP);
}

void DisplayGlfw::drawHelpLineSnake_() {
	actBlock_.resetTransform();
	actBlock_.translate(
			glm::vec3(yourSnakeX - winTileSize_.getX() / 2, yourSnakeY * -1 + winTileSize_.getY() / 2, 0.f));
	actBlock_.scale(glm::vec3(-0.10f));
	materialMap_.at(yourSnakeSprite & eSprite::kMaskColor).putMaterialToShader(shader_);

	ActModel line(actBlock_);
	eSprite to = (yourSnakeSprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo;
	if (to != eSprite::kNorth) {
		for (int y = yourSnakeY + 1; y < winTileSize_.getY(); ++y) {
			line.translate(glm::vec3(0.f, -1.f, 0.f));
			line.render(shader_);
		}
	}
	line.setPosition(actBlock_.getPosition());
	if (to != eSprite::kSouth) {
		for (int y = yourSnakeY; y; --y) {
			line.translate(glm::vec3(0.f, 1.f, 0.f));
			line.render(shader_);
		}
	}
	line.setPosition(actBlock_.getPosition());
	if (to != eSprite::kEast) {
		for (int x = yourSnakeX; x; --x) {
			line.translate(glm::vec3(-1.f, 0.f, 0.f));
			line.render(shader_);
		}
	}
	line.setPosition(actBlock_.getPosition());
	if (to != eSprite::kWest) {
		for (int x = yourSnakeX + 1; x < winTileSize_.getX(); ++x) {
			line.translate(glm::vec3(1.f, 0.f, 0.f));
			line.render(shader_);
		}
	}

}

void DisplayGlfw::render() {
	if (getKeyState(GLFW_KEY_H) == KeyState::kDown)
		flag_.flip(FLAG_LINE);


	if (indexActiveCamera_ == CAMERA_SNAKE) {
		if (getKeyState(GLFW_KEY_D) == KeyState::kDown) {
			if (((yourSnakeSprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo) == eSprite::kEast)
				direction_ = kSouth;
			else if (((yourSnakeSprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo) == eSprite::kSouth)
				direction_ = kWest;
			else if (((yourSnakeSprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo) == eSprite::kWest)
				direction_ = kNorth;
			else if (((yourSnakeSprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo) == eSprite::kNorth)
				direction_ = kEast;
		} else if (getKeyState(GLFW_KEY_A) == KeyState::kDown) {
			if (((yourSnakeSprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo) == eSprite::kEast)
				direction_ = kNorth;
			else if (((yourSnakeSprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo) == eSprite::kSouth)
				direction_ = kEast;
			else if (((yourSnakeSprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo) == eSprite::kWest)
				direction_ = kSouth;
			else if (((yourSnakeSprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo) == eSprite::kNorth)
				direction_ = kWest;
		}
	} else {
		if (getKey(GLFW_KEY_W))
			direction_ = kNorth;
		if (getKey(GLFW_KEY_S))
			direction_ = kSouth;
		if (getKey(GLFW_KEY_A))
			direction_ = kWest;
		if (getKey(GLFW_KEY_D))
			direction_ = kEast;
	}

	if (getKey(GLFW_KEY_RIGHT))
		camera_[CAMERA_MOVABLE].processPosition(Camera::Movement::RIGHT, deltaTime_ * 5);
	if (getKey(GLFW_KEY_LEFT))
		camera_[CAMERA_MOVABLE].processPosition(Camera::Movement::LEFT, deltaTime_ * 5);
	if (getKey(GLFW_KEY_DOWN))
		camera_[CAMERA_MOVABLE].processPosition(Camera::Movement::BACKWARD, deltaTime_ * 5);
	if (getKey(GLFW_KEY_UP))
		camera_[CAMERA_MOVABLE].processPosition(Camera::Movement::FORWARD, deltaTime_ * 5);

	if (getKeyState(GLFW_KEY_F1) == KeyState::kDown) {
		callback_(eAction::kSwitchDisplayLibrarySFML);
	}
	if (getKeyState(GLFW_KEY_F2) == KeyState::kDown) {
		callback_(eAction::kSwitchDisplayLibrarySDL);
	}
	if (getKeyState(GLFW_KEY_F3) == KeyState::kDown) {
		callback_(eAction::kSwitchDisplayLibraryGLFW);
	}
	if (getKeyState(GLFW_KEY_P) == KeyState::kDown) {
		callback_(eAction::kPause);
	}
	if (getKeyState(GLFW_KEY_O) == KeyState::kDown) {
		callback_(eAction::kSwitchDisplayLibrary);
	}

	if (getKey(GLFW_KEY_N) == KeyState::kDown) {
		activeNextCamera_();
	}

	shader_.activate();
	shader_.setInt("uBackground", 1);
	light_.putLightToShader(shader_);
	view_ = getActiveCamera_().getViewMatrix();
	shader_.setMat4("projection", projection_);
	shader_.setMat4("view", view_);
	shader_.setVec3("uCameraPosition", getActiveCamera_().getPosition());

	if (flag_.test(FLAG_LINE) && yourSnakeSprite != eSprite::kNone) {
		drawHelpLineSnake_();
	}
	shader_.setInt("uBackground", 0);

	shaderMultiple_.activate();
	light_.putLightToShader(shaderMultiple_);
	shaderMultiple_.setInt("uBackground", 1);
	shaderMultiple_.setVec3("uCameraPosition", getActiveCamera_().getPosition());
	shaderMultiple_.setMat4("projection", projection_);
	shaderMultiple_.setMat4("view", view_);


	materialMap_.at(eSprite::kGround).putMaterialToShader(shaderMultiple_);
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

void DisplayGlfw::registerCallbackAction(std::function<void(eAction)> function) {
	callback_ = function;
}

bool DisplayGlfw::exit() const {
	return Glfw::exit();
}

void DisplayGlfw::update() {
	Glfw::update();
	if (!cursor_ && indexActiveCamera_ == CAMERA_MOVABLE && Glfw::mouseCallbackCalled_) {
		camera_[CAMERA_MOVABLE].processMouseMovement(Glfw::offsetX_, Glfw::offsetY_);
		Glfw::mouseCallbackCalled_ = false;
	}
}

eDirection DisplayGlfw::getDirection() const {
	return (direction_);
}

DisplayGlfw::GlfwConstructorException::~GlfwConstructorException() noexcept = default;

DisplayGlfw::GlfwConstructorException::GlfwConstructorException() noexcept :
		error_("Error on Glfw constructor") {}

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(
		std::string const &s) noexcept :
		error_(s) {}

const char *
DisplayGlfw::GlfwConstructorException::what() const noexcept { return (error_.c_str()); }