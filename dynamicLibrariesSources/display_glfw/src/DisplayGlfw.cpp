#include <fstream>
#include "DisplayGlfw.hpp"
//#include <Carbon.framework/Headers/Carbon.h>

#define NEAR_PLANE 0.1f
#define MAX_PLANE 1000.f

#define CAMERA_GLOBAL 0
#define CAMERA_SNAKE 1

#define CAMERA_SIZE 2

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
		yourSnakeSprite(eSprite::NONE),
		yourSnakeX(-10),
		yourSnakeY(-10),
		projection_(1.f),
		indexActiveCamera_(0),
		view_(1.f),
		model_(1.f),
		light_(glm::vec3(0.f, 0.f, 30.f)),
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

	block_.setModel((pathRoot_ / "ressources" / "objects" / "nanosuit" / "nanosuit.obj").generic_string());
	modelGrass_.setModel((pathRoot_ / "ressources" / "objects" / "grass" / "grass.obj").generic_string());
	modelSphere_.setModel((pathRoot_ / "ressources" / "objects" / "sphere.obj").generic_string());
	modelHead_.setModel((pathRoot_ / "ressources" / "objects" / "head.obj").generic_string());
	modelWall_.setModel((pathRoot_ / "ressources" / "wall.obj").generic_string());
	appleModel_.setModel((pathRoot_ / "ressources" / "Apple_obj" / "apple.obj").generic_string());
	actBlock_.assign(&modelGrass_);


	camera_.reserve(CAMERA_SIZE);
	camera_.emplace_back();
	camera_.emplace_back();

	if (winTileSize_.getY() < winTileSize_.getX())
		camera_[CAMERA_GLOBAL].processPosition(Camera::Movement::BACKWARD, winTileSize_.getX() / 2);
	else
		camera_[CAMERA_GLOBAL].processPosition(Camera::Movement::BACKWARD, winTileSize_.getY() / 2);

	particuleBackground_ = new Particle((pathRoot_ / "ressources" / "objects" / "grass" / "grass.obj").generic_string(),
										winTileSize_.getY() * winTileSize_.getX());
	particuleBackgroundOutline_ = new Particle(
			(pathRoot_ / "ressources" / "objects" / "grass" / "grass.obj").generic_string(),
			winTileSize_.getY() * winTileSize_.getX());

}

void DisplayGlfw::constructMaterialMap_() {
	materialMap_.try_emplace(eSprite::GREEN, "GREEN");
	materialMap_.try_emplace(eSprite::GROUND, "GROUND", 0.f,
							 glm::vec3(0.0756f * 0.f, 0.4423f * 0.1f, 0.07568f * 0.1f),
							 glm::vec3(0.0756f, 0.5523f, 0.07568f),
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
							 glm::vec3(0.19f, 0.19f, 0.0545f),
							 glm::vec3(0.85f, 0.85f, 0.1f),
							 glm::vec3(0.628281f, 0.628281f, 0.628281f));
	glm::vec3 orange((244.f / 255.f), (81.f / 255.f), (30.f / 255));
	materialMap_.try_emplace(eSprite::ORANGE, "ORANGE", 50.f, orange * 0.1f, orange, orange);
	materialMap_.try_emplace(eSprite::RED, "RED", 31.f,
							 glm::vec3(0.2f, 0.07f, 0.07f),
							 glm::vec3(0.7f, 0.07f, 0.07f),
							 glm::vec3(0.7f, 0.6f, 0.6f));
	materialMap_.try_emplace(eSprite::FOOD, "FOOD", 76.8f,
							 glm::vec3(0.1745f, 0.01175f, 0.01175f),
							 glm::vec3(0.61424f, 0.04136f, 0.04136f),
							 glm::vec3(0.727811f, 0.626959f, 0.626959f));
	materialMap_.try_emplace(eSprite::WALL, "WALL", 0.f,
							 glm::vec3(0.05f, 0.05f, 0.05f),
							 glm::vec3(0.2f, 0.2f, 0.2f),
							 glm::vec3(0.2f, 0.2f, 0.2f));
	materialMap_.try_emplace(eSprite::NONE, "NONE", 0.f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
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

	if ((sprite & eSprite::MASK_BODY) == eSprite::TAIL)
		grid_(x, y).scale(glm::vec3(-0.1f));
	else if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD) {
		eSprite to = (sprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;

		ActModel eyeLeft = grid_(x, y);
		ActModel eyeRight = grid_(x, y);

		if (to == eSprite::EAST) {
			eyeLeft.translate(glm::vec3(0.30f, 0.15f, 0.05f));
			eyeRight.translate(glm::vec3(0.30f, -0.15f, 0.05f));
		} else if (to == eSprite::WEST) {
			eyeLeft.translate(glm::vec3(-0.30f, 0.15f, 0.05f));
			eyeRight.translate(glm::vec3(-0.30f, -0.15f, 0.05f));
		} else if (to == eSprite::NORTH) {
			eyeLeft.translate(glm::vec3(0.15f, 0.3f, 0.05f));
			eyeRight.translate(glm::vec3(-0.15f, 0.3f, 0.05f));
		} else if (to == eSprite::SOUTH) {
			eyeLeft.translate(glm::vec3(0.15f, -0.3f, 0.05f));
			eyeRight.translate(glm::vec3(-0.15f, -0.3f, 0.05f));
		}
		eyeLeft.scale(glm::vec3(-0.8f));
		eyeRight.scale(glm::vec3(-0.8f));

		Material::unsetMaterial(shader_);
		eyeLeft.render(shader_);
		eyeRight.render(shader_);
		materialMap_.at(sprite & eSprite::MASK_COLOR).putMaterialToShader(shader_);
		if ((sprite & eSprite::YOUR_SNAKE) == eSprite::YOUR_SNAKE) {
			yourSnakeSprite = sprite;
			yourSnakeX = x;
			yourSnakeY = y;
			camera_[CAMERA_SNAKE].setPosition(grid_(x, y).getPosition() + glm::vec3(0.f, 0.f, 10.f));
			if (to == eSprite::EAST)
				camera_[CAMERA_SNAKE].setPosition(camera_[CAMERA_SNAKE].getPosition() + glm::vec3(-20.f, 0.f, 0.f));
			else if (to == eSprite::WEST)
				camera_[CAMERA_SNAKE].setPosition(camera_[CAMERA_SNAKE].getPosition() + glm::vec3(20.f, 0.f, 0.f));
			else if (to == eSprite::NORTH)
				camera_[CAMERA_SNAKE].setPosition(camera_[CAMERA_SNAKE].getPosition() + glm::vec3(0.f, -20.f, 0.f));
			else if (to == eSprite::SOUTH)
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
	if (static_cast<int>(sprite & eSprite::MASK_BODY) != 0) {
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


	for (int y = 0; y < winTileSize_.getY(); ++y) {
		for (int x = 0; x < winTileSize_.getX(); ++x) {
			if ((grid(x, y) & eSprite::FOOD) == eSprite::FOOD) {
				materialMap_.at(eSprite::FOOD).putMaterialToShader(shader_);
				grid_(x, y).assign(&appleModel_);
			} else if (static_cast<int>(grid(x, y) & eSprite::MASK_BODY) != 0) {
				grid_(x, y).assign(&modelSphere_);
				materialMap_.at(grid(x, y) & eSprite::MASK_COLOR).putMaterialToShader(shader_);
			} else if ((grid(x, y) & eSprite::WALL) == eSprite::WALL) {
				grid_(x, y).assign(&modelWall_);
				materialMap_.at(eSprite::WALL).putMaterialToShader(shader_);
			} else if (grid(x, y) != eSprite::NONE)
				grid_(x, y).assign(&block_);

			if (grid(x, y) != eSprite::NONE)
				drawGridCase_(grid(x, y), x, y);

			if ((grid(x, y) & eSprite::WALL) == eSprite::WALL)
				renderLine_(grid_(x, y));
		}
	}
}

void DisplayGlfw::interpolateGridCase_(int x, int y) {
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

void DisplayGlfw::drawHelpLineSnake_() {
	actBlock_.resetTransform();
	actBlock_.translate(glm::vec3(yourSnakeX - winTileSize_.getX() / 2, yourSnakeY * -1 + winTileSize_.getY() / 2, 0.f));
	actBlock_.scale(glm::vec3(-0.10f));
	materialMap_.at(yourSnakeSprite & eSprite::MASK_COLOR).putMaterialToShader(shader_);

	ActModel line(actBlock_);
	eSprite to = (yourSnakeSprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;
	if (to != eSprite::NORTH) {
		for (int y = yourSnakeY + 1; y < winTileSize_.getY(); ++y) {
			line.translate(glm::vec3(0.f, -1.f, 0.f));
			line.render(shader_);
		}
	}
	line.setPosition(actBlock_.getPosition());
	if (to != eSprite::SOUTH) {
		for (int y = yourSnakeY; y; --y) {
			line.translate(glm::vec3(0.f, 1.f, 0.f));
			line.render(shader_);
		}
	}
	line.setPosition(actBlock_.getPosition());
	if (to != eSprite::EAST) {
		for (int x = yourSnakeX; x; --x) {
			line.translate(glm::vec3(-1.f, 0.f, 0.f));
			line.render(shader_);
		}
	}
	line.setPosition(actBlock_.getPosition());
	if (to != eSprite::WEST) {
		for (int x = yourSnakeX + 1; x < winTileSize_.getX(); ++x) {
			line.translate(glm::vec3(1.f, 0.f, 0.f));
			line.render(shader_);
		}
	}

}

void DisplayGlfw::render(float currentDelayFrame, float maxDelayFrame) {
	currentTimer_ = currentDelayFrame;
	maxTimer_ = maxDelayFrame;

	if (getKeyState(GLFW_KEY_H) == KeyState::kDown)
		flag_.flip(FLAG_LINE);


	if (indexActiveCamera_ == CAMERA_SNAKE) {
		if (getKeyState(GLFW_KEY_RIGHT) == KeyState::kDown) {
			if (((yourSnakeSprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO) == eSprite::EAST)
				direction_ = kSouth;
			else if (((yourSnakeSprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO) == eSprite::SOUTH)
				direction_ = kWest;
			else if (((yourSnakeSprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO) == eSprite::WEST)
				direction_ = kNorth;
			else if (((yourSnakeSprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO) == eSprite::NORTH)
				direction_ = kEast;
		} else if (getKeyState(GLFW_KEY_LEFT) == KeyState::kDown) {
			if (((yourSnakeSprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO) == eSprite::EAST)
				direction_ = kNorth;
			else if (((yourSnakeSprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO) == eSprite::SOUTH)
				direction_ = kEast;
			else if (((yourSnakeSprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO) == eSprite::WEST)
				direction_ = kSouth;
			else if (((yourSnakeSprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO) == eSprite::NORTH)
				direction_ = kWest;
		}
	} else {
		if (getKey(GLFW_KEY_UP))
			direction_ = kNorth;
		if (getKey(GLFW_KEY_DOWN))
			direction_ = kSouth;
		if (getKey(GLFW_KEY_LEFT))
			direction_ = kWest;
		if (getKey(GLFW_KEY_RIGHT))
			direction_ = kEast;
	}

	if (getKey(GLFW_KEY_D))
		camera_[CAMERA_GLOBAL].processPosition(Camera::Movement::RIGHT, deltaTime_ * 5);
	if (getKey(GLFW_KEY_A))
		camera_[CAMERA_GLOBAL].processPosition(Camera::Movement::LEFT, deltaTime_ * 5);
	if (getKey(GLFW_KEY_S))
		camera_[CAMERA_GLOBAL].processPosition(Camera::Movement::BACKWARD, deltaTime_ * 5);
	if (getKey(GLFW_KEY_W))
		camera_[CAMERA_GLOBAL].processPosition(Camera::Movement::FORWARD, deltaTime_ * 5);

	if (getKeyState(GLFW_KEY_P) == KeyState::kDown) {
		callback_(eAction::kPause);
	}
	if (getKeyState(GLFW_KEY_O) == KeyState::kDown) {
		callback_(eAction::kSwitchDisplayLibrary);
	}

	if (getKey(GLFW_KEY_N) == KeyState::kDown)
		activeNextCamera_();
	if (getKey(GLFW_KEY_B) == KeyState::kDown) {
		camera_[CAMERA_GLOBAL].setPosition(camera_[CAMERA_SNAKE].getPosition());
	}

	shader_.activate();
	shader_.setInt("uBackground", 1);
	light_.putLightToShader(shader_);
	view_ = getActiveCamera_().getViewMatrix();
	shader_.setMat4("projection", projection_);
	shader_.setMat4("view", view_);
	shader_.setVec3("uCameraPosition", getActiveCamera_().getPosition());

	if (flag_.test(FLAG_LINE) && yourSnakeSprite != eSprite::NONE) {
		drawHelpLineSnake_();
	}
	shader_.setInt("uBackground", 0);

	shaderMultiple_.activate();
	light_.putLightToShader(shaderMultiple_);
	shaderMultiple_.setInt("uBackground", 1);
	shaderMultiple_.setVec3("uCameraPosition", getActiveCamera_().getPosition());
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

void DisplayGlfw::registerCallbackAction(std::function<void(eAction)> function) {
	callback_ = function;
}

bool DisplayGlfw::exit() const {
	return Glfw::exit();
}

void DisplayGlfw::update(float deltaTime) {
	deltaTime_ = deltaTime;
	Glfw::update();
	if (!cursor_ && Glfw::mouseCallbackCalled_) {
		camera_[CAMERA_GLOBAL].processMouseMovement(Glfw::offsetX_, Glfw::offsetY_);
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
		std::string s) noexcept :
		error_(s) {}

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(
		DisplayGlfw::GlfwConstructorException const &src) noexcept :
		error_(src.error_) { error_ = src.error_; }

const char *
DisplayGlfw::GlfwConstructorException::what() const noexcept { return (error_.c_str()); }