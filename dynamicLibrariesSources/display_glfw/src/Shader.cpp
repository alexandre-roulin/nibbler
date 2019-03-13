#include "Shader.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>

Shader::Shader() noexcept :
		program_(glCreateProgram()) {
}

Shader::Shader(std::string const &file1, std::string const &file2) :
		program_(glCreateProgram()) {
	attach(file1);
	attach(file2);
	link();
}

Shader::~Shader() noexcept {
	clean_();
}

void		Shader::clean_() noexcept {
	glDeleteProgram(program_);
}

Shader &Shader::activate() noexcept {
	glUseProgram(program_);
	return (*this);
}

Shader	&Shader::attach(std::string const &filename) {
	// Get Shader source
	std::ifstream fd(filename);
	std::string src = std::string(std::istreambuf_iterator<char>(fd),
						   (std::istreambuf_iterator<char>()));
	const char *source = src.c_str();

	auto shader = create(filename);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status_);

	if (!status_) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, & length_);
		std::unique_ptr<char[]> buffer(new char[length_]);
		glGetShaderInfoLog(shader, length_, nullptr, buffer.get());
		if (Shader::debug_)
			std::cerr << filename.c_str() << std::endl << buffer.get() << std::endl;
		glDeleteShader(shader);
		throw (Shader::CreateException(filename + buffer.get()));
	}
	
	glAttachShader(program_, shader);
	glDeleteShader(shader);
	return (*this);
}

GLuint Shader::create(std::string const &filename)
{
	auto index = filename.rfind('.');
	auto ext = filename.substr(index + 1);

	if (ext == "comp")
		return glCreateShader(GL_COMPUTE_SHADER);
	else if (ext == "frag")
		return glCreateShader(GL_FRAGMENT_SHADER);
	else if (ext == "geom")
		return glCreateShader(GL_GEOMETRY_SHADER);
	else if (ext == "vert")
		return glCreateShader(GL_VERTEX_SHADER);
	return static_cast<GLuint>(false);
}

Shader &Shader::link()
{
	glLinkProgram(program_);
	glGetProgramiv(program_, GL_LINK_STATUS, &status_);
	if(!status_) {
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH, & length_);
		std::unique_ptr<char[]> buffer(new char[length_]);
		glGetProgramInfoLog(program_, length_, nullptr, buffer.get());
		if (Shader::debug_)
			fprintf(stderr, "%s", buffer.get());
		throw (Shader::LinkException(buffer.get()));
	}
	return *this;
}

void		Shader::setFloat(const std::string &name, float value) const {
	GLint uniform = glGetUniformLocation(program_, name.c_str());
	if (uniform == -1)
		throw (std::invalid_argument(std::string("glGetUniformLocation::setFloat failed [") + name + "]"));
	glUniform1f(uniform, value);
}
void		Shader::setMat4(const std::string &name, const glm::mat4 &mat) const  {
	GLint uniform = glGetUniformLocation(program_, name.c_str());
	if (uniform == -1)
		throw (std::invalid_argument(std::string("glGetUniformLocation::setMat4 failed [") + name + "]"));
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat[0][0]);
}
void		Shader::setVec3(const std::string &name, const glm::vec3 &vec) const  {
	GLint uniform = glGetUniformLocation(program_, name.c_str());
	if (uniform == -1)
		throw (std::invalid_argument(std::string("glGetUniformLocation::setVec3 failed [") + name + "]"));
	glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, &vec[0]);
}
void		Shader::setInt(const std::string &name, int i) const  {
	GLint uniform = glGetUniformLocation(program_, name.c_str());
	if (uniform == -1)
		throw (std::invalid_argument(std::string("glGetUniformLocation::setInt failed [") + name + "]"));
	glUniform1i(uniform, i);
}

GLuint 		Shader::getId() const {
	return (program_);
}

bool				Shader::debug_ = true;


Shader::CreateException::~CreateException() noexcept = default;
Shader::CreateException::CreateException() noexcept :
		invalid_argument(this->_error),
		_error("You make a CreateException") {}
Shader::CreateException::CreateException(std::string s) noexcept :
		invalid_argument(s),
		_error(s) { }
Shader::CreateException::CreateException(Shader::CreateException const &src) noexcept :
		invalid_argument(this->_error),
		_error(src._error)
{ this->_error = src._error; }
const char	*Shader::CreateException::what() const noexcept
{ return (this->_error.c_str()); }

Shader::LinkException::~LinkException() noexcept = default;
Shader::LinkException::LinkException() noexcept :
		invalid_argument(this->_error),
		_error("You make a LinkException") {}
Shader::LinkException::LinkException(std::string s) noexcept :
		invalid_argument(s),
		_error(s) { }
Shader::LinkException::LinkException(Shader::LinkException const &src) noexcept :
		invalid_argument(this->_error),
		_error(src._error)
{ this->_error = src._error; }
const char	*Shader::LinkException::what() const noexcept
{ return (this->_error.c_str()); }