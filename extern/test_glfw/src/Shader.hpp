#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:
    Shader();
    ~Shader();

    Shader		&activate();
	Shader		&attach(std::string const &filename);
	GLuint		create(std::string const &filename);
	Shader		&link();


private:

	GLuint program_;
	GLint  status_;
	GLint  length_;

	void	clean_();

	Shader(Shader const &shader) = delete;
    Shader &operator=(Shader const &shader) = delete;
};