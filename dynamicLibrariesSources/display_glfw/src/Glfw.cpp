#include "Glfw.hpp"

Glfw::Glfw(std::string const &name, uint16_t width, uint16_t height) :
    cursor_(true) {
    glfwSetErrorCallback(Glfw::callbackError_);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    if (!(window_ = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr))) {
        clean_();
		throw (Glfw::ConstructorException("GlfwConstructorException: window was not created"));
    }
    glfwMakeContextCurrent(window_);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(0);

	glfwSetKeyCallback(window_, Glfw::callbackKey_);
    glfwSetCursorPosCallback(window_, Glfw::mouseCallback_);

    Glfw::glfwByWindow_.insert(std::pair<GLFWwindow*, Glfw&>(window_, *this));
}

Glfw::~Glfw() {
    Glfw::glfwByWindow_.erase(Glfw::glfwByWindow_.find(window_));
    clean_();
}

void    Glfw::clean_() {
    if (window_) {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

void			Glfw::callbackKey_(GLFWwindow* window, int key, int scancode, int action, int mods) {
    for (auto &glfw : Glfw::glfwByWindow_) {
        if (glfw.first == window) {
            if (action == GLFW_REPEAT) {
				glfw.second.callbackKey(key, KeyState::kPress);
            }
            else if (action == GLFW_RELEASE) {
				glfw.second.callbackKey(key, KeyState::kUp);
            }
            else if (action == GLFW_PRESS) {
				glfw.second.callbackKey(key, KeyState::kDown);

            }
        }
    }
}
void Glfw::callbackError_(int error, const char* errorMessage) {
    throw (Glfw::ConstructorException(errorMessage));
}

void            Glfw::update() {

	KeyStateManager::update();

	if (getKeyState(GLFW_KEY_ESCAPE) == KeyState::kDown)
        glfwSetWindowShouldClose(window_, true);
    if (getKeyState(GLFW_KEY_SPACE) == KeyState::kDown) {
        if (cursor_)
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursor_ = !cursor_;
    }
}

void            Glfw::render() {
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

bool            Glfw::exit() const {
    return static_cast<bool>(glfwWindowShouldClose(window_));
}

GLFWwindow     *Glfw::getWindow() const {
    return (window_);
}

std::map<GLFWwindow*, Glfw&>     Glfw::glfwByWindow_;

Glfw::ConstructorException::~ConstructorException(void) noexcept{}
Glfw::ConstructorException::ConstructorException(void) noexcept :
        _error("Error on Glfw constructor") {}
Glfw::ConstructorException::ConstructorException(std::string s) noexcept :
        _error(s) { }
Glfw::ConstructorException::ConstructorException(Glfw::ConstructorException const &src) noexcept :
        _error(src._error)
{ this->_error = src._error; }
const char	*Glfw::ConstructorException::what() const noexcept
{ return (this->_error.c_str()); }

bool        Glfw::firstMouse_ = true;
bool        Glfw::mouseCallbackCalled_ = false;
float       Glfw::lastX_ = DISPLAY_GLFW_WIN_WIDTH / 2.0f;
float       Glfw::lastY_ = DISPLAY_GLFW_WIN_HEIGHT / 2.0f;
float       Glfw::offsetX_ = 0.f;
float       Glfw::offsetY_ = 0.f;

void Glfw::mouseCallback_(GLFWwindow *window, double xpos, double ypos) {
    if (Glfw::firstMouse_) {
        Glfw::lastX_ = static_cast<float>(xpos);
        Glfw::lastY_ = static_cast<float>(ypos);
        Glfw::firstMouse_ = false;
    }

    Glfw::offsetX_ = static_cast<float>(xpos - Glfw::lastX_);
    Glfw::offsetY_ = static_cast<float>(Glfw::lastY_ - ypos);

    Glfw::lastX_ = static_cast<float>(xpos);
    Glfw::lastY_ = static_cast<float>(ypos);
    mouseCallbackCalled_ = true;
}