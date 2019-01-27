#include "DisplayGlfw.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
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
#include <MutantGrid.tpp>
int main(int argc, char **argv) {
	try {
		DisplayGlfw lol(35, 35, "Issou");

		MutantGrid< eSprite > background(35);
		MutantGrid< eSprite > al(35);

		background.fill(eSprite::GROUND);

		al.fill(eSprite::NONE);

		al(0, 0) = eSprite::FOOD;
		al(1, 1) = eSprite::WALL;
		al(3, 3) = (eSprite::BODY | eSprite::BLUE);
		al(4, 3) = (eSprite::HEAD | eSprite::GREEN);
		al(5, 3) = (eSprite::HEAD | eSprite::PURPLE);
		al(6, 3) = (eSprite::HEAD | eSprite::PINK);
		al(7, 3) = (eSprite::HEAD | eSprite::GREY);
		al(8, 3) = (eSprite::HEAD | eSprite::YELLOW);
		al(9, 3) = (eSprite::HEAD | eSprite::ORANGE);
		al(10, 3) = (eSprite::HEAD | eSprite::RED);

		al(9, 1) = (eSprite::WALL);
		al(1, 9) = eSprite::WALL;


		lol.setBackground(background);

		float deltaTime = 0.0f;
		float lastFrame = 0.0f;



		float dist = 0;
		float maxDist = 1.4f;

		float start = 0.f;

		float count = 0.f;

		bool f = true;

		while (!lol.exit()) {

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			dist += deltaTime;
			if (f) {
				f = false;
			} else {
				start += deltaTime;
			}


			if (dist > maxDist) {
				dist = 0;
			}

			//std::cout << deltaTime << std::endl;

			lol.update(deltaTime);
			lol.drawGrid(al);
			lol.render(dist, maxDist);

			count += 1.f;


			std::cout << "It took me " << deltaTime << " seconds." << std::endl;
		}
		std::cout << "Average " << start << " for " << count << " So : " << (start / count) << " seconds.";

	} catch (std::exception const &e) {
		std::cout << e.what() << std::endl;
	}

/*
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


	bool second = true;
	float time = 0.f;

	model.translate(glm::vec3(0.f, 0.f, 0.5f));

	glm::vec3 nextDirection = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 currentDirection = glm::vec3(0.f, 1.f, 0.f);

	while (!glfw.exit()) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		time += deltaTime;
		if (time > 1.f) {
			time -= 1.f;
			second = true;
			currentDirection = nextDirection;
		}

		view = camera.getViewMatrix();
		m_model = model.getTransform();

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

		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_I) == GLFW_PRESS)
			model.translate(glm::vec3(0.f, 0.f, 1.f) * deltaTime);
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_K) == GLFW_PRESS)
			model.translate(glm::vec3(0.f, 0.f, -1.f) * deltaTime);
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_J) == GLFW_PRESS)
			model.translate(glm::vec3(1.f, 0.f, 0.f) * deltaTime);
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_L) == GLFW_PRESS)
			model.translate(glm::vec3(-1.f, 0.f, 0.f) * deltaTime);

		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_T) == GLFW_PRESS)
			model.rotate(glm::vec3(1.f, 0.f, 0.f), deltaTime);
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_G) == GLFW_PRESS)
			model.rotate(glm::vec3(0.f, 1.f, 0.f), deltaTime);
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_B) == GLFW_PRESS)
			model.rotate(glm::vec3(0.f, 0.f, 1.f), deltaTime);


		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_KP_8) == GLFW_PRESS) {
			nextDirection = glm::vec3(0.f, 1.f, 0.f);
		}
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_KP_5) == GLFW_PRESS) {
			nextDirection = glm::vec3(0.f, -1.f, 0.f);
		}
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_KP_4) == GLFW_PRESS) {
			nextDirection = glm::vec3(-1.f, 0.f, 0.f);
		}
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_KP_6) == GLFW_PRESS) {
			nextDirection = glm::vec3(1.f, 0.f, 0.f);
		}

		model.translate(currentDirection * deltaTime);



		m_model = model.getTransform();



		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", m_model);
		model.render(shader);

		for (int y = -25; y < 25; y++) {
			for (int x = -25; x < 25; x++) {
				block.resetTransform();
				block.translate(glm::vec3(x, y, 0.f));
				block.scale(glm::vec3(-0.10f));
				m_model = block.getTransform();
				shader.setMat4("model", m_model);

				block.render(shader);
			}
		}

        glfw.render();
    }*/

    return (0);

}