#include "Skybox.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <stb_image.h>
#include <vector>
#include <glm/glm.hpp>

Skybox::Skybox(std::string const &pathShader,
				std::string const &pathDirectorySkyBox,
				std::list< std::string > const &skyboxFile) {

	if (skyboxFile.size() < SKYBOX_NUMBER_OF_FACES)
		throw (Skybox::ConstructorException("Missing file during construct Skybox"));

	shader_.attach(pathShader + ".vert");
	shader_.attach(pathShader + ".frag");
	shader_.link();

	glGenTextures(1, &textureSkyBox_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkyBox_);

	int width, height, nrChannels;
	std::list<std::string>::const_iterator it = skyboxFile.begin();
	for (unsigned int i = 0; i < SKYBOX_NUMBER_OF_FACES && it != skyboxFile.end(); i++, it++) {
		unsigned char *data = stbi_load((pathDirectorySkyBox + *it).c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			std::cout << "loadSkyBox_ : " << (pathDirectorySkyBox + *it).c_str() << std::endl;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			stbi_image_free(data);
			throw (Skybox::ConstructorException(std::string("loadSkyBox_ : ") + pathDirectorySkyBox + (*it).c_str() + " failed."));
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenVertexArrays(1, &skyboxVAO_);
	glGenBuffers(1, &skyboxVBO_);
	glBindVertexArray(skyboxVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices_), &skyboxVertices_, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
}

Skybox::~Skybox() noexcept = default;


void Skybox::render(glm::mat4 const &view, glm::mat4 const &projection) {
	/* Remove Translation */
	glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(view));

	glDepthFunc(GL_LEQUAL);
	shader_.activate();
	shader_.setMat4("view", viewWithoutTranslation);
	shader_.setMat4("projection", projection);
	glBindVertexArray(skyboxVAO_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkyBox_);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


float	Skybox::skyboxVertices_[6 * 6 * 3] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
};

Skybox::ConstructorException::~ConstructorException(void) noexcept{}
Skybox::ConstructorException::ConstructorException(void) noexcept :
		invalid_argument(this->_error),
		_error("You make a ConstructorException") {}
Skybox::ConstructorException::ConstructorException(std::string s) noexcept :
		invalid_argument(s),
		_error(s) { }
Skybox::ConstructorException::ConstructorException(Skybox::ConstructorException const &src) noexcept :
		invalid_argument(this->_error),
		_error(src._error)
{ this->_error = src._error; }
const char	*Skybox::ConstructorException::what() const noexcept
{ return (this->_error.c_str()); }