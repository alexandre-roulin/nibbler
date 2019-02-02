#pragma once

#include <glad/glad.h>
#include <string>
#include <list>
#include "Shader.hpp"

# define SKYBOX_NUMBER_OF_FACES 6

class Skybox {
public:
	class ConstructorException : public std::invalid_argument {
	public:
		ConstructorException(void) noexcept;
		ConstructorException(std::string) noexcept;
		virtual const char* what() const noexcept;
		~ConstructorException(void) throw();
		ConstructorException(ConstructorException const &src) noexcept;
	private:
		ConstructorException &operator=(ConstructorException const &rhs) noexcept;
		std::string			_error;
	};

	enum eFace { RIGHT = 0, LEFT, UP, DOWN, FRONT, BACK };

	Skybox(std::string const &pathSkybox,
			std::string const &pathDirectorySkyBox,
			std::list< std::string > const &skyboxFile);
	~Skybox() noexcept;
	Skybox() noexcept = delete;
	Skybox(Skybox const &shader) = delete;
	Skybox &operator=(Skybox const &shader) = delete;

	void render(glm::mat4 const &view, glm::mat4 const &projection);

private:

	Shader							shader_;
	unsigned int					textureSkyBox_;
	unsigned int					skyboxVAO_;
	unsigned int					skyboxVBO_;

	/* 6 Faces, 6 Points (3 per traingle), 3 coordinate per point */
	static float	skyboxVertices_[6 * 6 * 3];
};