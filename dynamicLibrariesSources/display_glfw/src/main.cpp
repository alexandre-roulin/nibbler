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
		al(3, 3) = (eSprite::BODY | eSprite::BLUE | eSprite::EAST);
		al(4, 3) = (eSprite::HEAD | eSprite::GREEN | eSprite::WEST);
		al(5, 3) = (eSprite::HEAD | eSprite::PURPLE | eSprite::SOUTH);
		al(6, 3) = (eSprite::HEAD | eSprite::PINK | eSprite::NORTH);
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


			//std::cout << "It took me " << deltaTime << " seconds." << std::endl;
		}
		std::cout << "Average " << start << " for " << count << " So : " << (start / count) << " seconds." << std::endl;

	} catch (std::exception const &e) {
		std::cout << e.what() << std::endl;
	}

    return (0);

}