#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:
	class CreateException : public std::invalid_argument {
	public:
		CreateException(void) noexcept;
		CreateException(std::string) noexcept;
		virtual const char* what() const noexcept;
		~CreateException(void) throw();
		CreateException(CreateException const &src) noexcept;
	private:
		CreateException &operator=(CreateException const &rhs) noexcept;
		std::string			_error;
	};
	class LinkException : public std::invalid_argument {
	public:
		LinkException(void) noexcept;
		LinkException(std::string) noexcept;
		virtual const char* what() const noexcept;
		~LinkException(void) throw();
		LinkException(LinkException const &src) noexcept;
	private:
		LinkException &operator=(LinkException const &rhs) noexcept;
		std::string			_error;
	};
	
    Shader() noexcept;
	Shader(std::string const &file1, std::string const &file2);
	~Shader() noexcept;

    Shader		&activate() noexcept;
	Shader		&attach(std::string const &filename);
	GLuint		create(std::string const &filename);
	Shader		&link();

private:

	GLuint program_;
	GLint  status_;
	GLint  length_;

	void	clean_() noexcept;
	
	static bool				debug_;
	
	Shader(Shader const &shader) = delete;
    Shader &operator=(Shader const &shader) = delete;
};