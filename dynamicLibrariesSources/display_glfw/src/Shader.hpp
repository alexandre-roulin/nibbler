#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:
	class CreateException : public std::invalid_argument {
	public:
		CreateException() noexcept;
		CreateException(std::string) noexcept;
		virtual const char* what() const noexcept;
		~CreateException() noexcept;
		CreateException(CreateException const &src) noexcept;
	private:
		CreateException &operator=(CreateException const &rhs) noexcept;
		std::string			_error;
	};
	class LinkException : public std::invalid_argument {
	public:
		LinkException() noexcept;
		explicit LinkException(std::string) noexcept;
		const char* what() const noexcept override;
		~LinkException() noexcept override;
		LinkException(LinkException const &src) noexcept;
	private:
		LinkException &operator=(LinkException const &rhs) noexcept;
		std::string			_error;
	};
	
    Shader() noexcept;
	Shader(std::string const &file1, std::string const &file2);
	~Shader() noexcept;
	Shader(Shader const &shader) = delete;
	Shader &operator=(Shader const &shader) = delete;

    Shader		&activate() noexcept;
	Shader		&attach(std::string const &filename);
	GLuint		create(std::string const &filename);
	Shader		&link();

	void		setFloat(const std::string &name, float value) const;
	void		setMat4(const std::string &name, const glm::mat4 &mat) const;
	void		setInt(const std::string &name, int aint) const;
	void		setVec3(const std::string &name, const glm::vec3 &vec) const;

	GLuint 		getId() const;

private:

	GLuint program_;
	GLint  status_;
	GLint  length_;

	void	clean_() noexcept;
	
	static bool				debug_;
};