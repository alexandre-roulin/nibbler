#include "DisplayGlfw.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

#include "Glfw.hpp"
#include "Model.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include <fstream>


#define WIN_WIDTH 1024
#define WIN_HEIGHT 720

bool firstMouse = true;
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;
Camera camera;
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}


int main(int argc, char **argv) {
    DisplayGlfw lol(NULL, 35, 32, 32, "Issou");


    lol.update();
    Glfw glfw("Coucou", WIN_WIDTH, WIN_HEIGHT);


	glEnable(GL_DEPTH_TEST);
	glPointSize(5.0);
	glDepthFunc(GL_LESS);

    Shader shader;


	std::string file_path = __FILE__;

#ifdef __APPLE__

	std::string dir_path = file_path.substr(0, file_path.rfind("/"));
    dir_path = dir_path.substr(0, dir_path.rfind("/"));
    std::cout << dir_path << std::endl;

	std::ifstream t(dir_path + "/file.txt");
	std::string path_model((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());


    shader.attach(dir_path + "/shader/basic.frag");
    std::cout << "Frag passed" << std::endl;
    shader.attach(dir_path + "/shader/basic.vert");
    std::cout << "Vert passed" << std::endl;

#else
	std::string dir_path = file_path.substr(0, file_path.rfind("\\"));
	dir_path = dir_path.substr(0, dir_path.rfind("\\"));
	std::cout << dir_path << std::endl;

	std::ifstream t(dir_path + "\\file.txt");
	std::string path_model((std::istreambuf_iterator<char>(t)),
						   std::istreambuf_iterator<char>());


	shader.attach(dir_path + "\\shader\\basic.frag");
	std::cout << "Frag passed" << std::endl;
	shader.attach(dir_path + "\\shader\\basic.vert");
	std::cout << "Vert passed" << std::endl;
#endif

	shader.link();
	std::cout << "<Linked passed" << std::endl;

	std::cout << path_model << std::endl;

	Model model(path_model);

	glm::mat4 projection = glm::mat4(1.0);
	projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100000.0f);
	glm::mat4 view = glm::mat4(1.0);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.0f));
	glm::mat4 m_model = glm::mat4(1.0);
	m_model = glm::translate(m_model, glm::vec3(0.0f, -10.0f, -30.0f));


	glfwSetCursorPosCallback(glfw.getWindow(), mouseCallback);


	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

    while (!glfw.exit()) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		view = camera.getViewMatrix();


		camera.update();
		glfw.update();
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		shader.activate();

		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
			camera.processPosition(Camera::Movement::FORWARD, deltaTime);
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
			camera.processPosition(Camera::Movement::BACKWARD, deltaTime);
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
			camera.processPosition(Camera::Movement::LEFT, deltaTime);
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
			camera.processPosition(Camera::Movement::RIGHT, deltaTime);
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_F) == GLFW_PRESS) {
		}

		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_G) == GLFW_PRESS)
			m_model = glm::rotate(m_model, glm::radians(-1.0f), glm::vec3(0.f, -1.0f, -0.0f));

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", m_model);

		GLint total_mem_kb = 0;
		glGetIntegerv(0x9048, &total_mem_kb);
		GLint cur_avail_mem_kb = 0;
		glGetIntegerv(0x9049, &cur_avail_mem_kb);
		//std::cout << "total_mem_kb : " << total_mem_kb << std::endl;
		//std::cout << "cur_avail_mem_kb : " << cur_avail_mem_kb << std::endl;

		model.render(shader);

        glfw.render();
    }

    return (0);

}