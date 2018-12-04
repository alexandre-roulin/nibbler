#include "Shader.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>

Shader::Shader() :
program_(glCreateProgram()) {
}

Shader::~Shader() {
	clean_();
}

void		Shader::clean_() {
	glDeleteProgram(program_);
}

Shader &Shader::activate() {
	glUseProgram(program_);
	return (*this);
}

Shader	&Shader::attach(std::string const &filename) {
	// Get Shader source
	std::ifstream fd(filename);
	auto src = std::string(std::istreambuf_iterator<char>(fd),
						   (std::istreambuf_iterator<char>()));

	// Create a Shader Object
	const char *source = src.c_str();
	std::cout << "Source path :" <<  filename << std::endl;
	std::cout << source << std::endl;
	auto shader = create(filename);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status_);

	// Display the Build Log on Error
	if (status_ == false) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, & length_);
		std::unique_ptr<char[]> buffer(new char[length_]);
		glGetShaderInfoLog(shader, length_, nullptr, buffer.get());
		std::cerr << filename.c_str() << std::endl << buffer.get() << std::endl;
	}

	// Attach the Shader and Free Allocated Memory
	glAttachShader(program_, shader);
	glDeleteShader(shader);
	assert(status_ == true);
	return (*this);
}

GLuint Shader::create(std::string const &filename)
{
	auto index = filename.rfind(".");
	auto ext = filename.substr(index + 1);

	if (ext == "comp")
		return glCreateShader(GL_COMPUTE_SHADER);
	else if (ext == "frag")
		return glCreateShader(GL_FRAGMENT_SHADER);
	else if (ext == "geom")
		return glCreateShader(GL_GEOMETRY_SHADER);
	else if (ext == "vert")
		return glCreateShader(GL_VERTEX_SHADER);
	return false;
}

Shader &Shader::link()
{
	glLinkProgram(program_);
	glGetProgramiv(program_, GL_LINK_STATUS, &status_);
	if(status_ == false) {
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH, & length_);
		std::unique_ptr<char[]> buffer(new char[length_]);
		glGetProgramInfoLog(program_, length_, nullptr, buffer.get());
		fprintf(stderr, "%s", buffer.get());
	}
	assert(status_ == true);
	return *this;
}