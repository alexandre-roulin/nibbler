#pragma  once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.hpp"

class Shader;

struct Vertex {

	Vertex() noexcept :
			position(glm::vec3(0.0f)),
			normal(glm::vec3(0.0f)),
			uv(glm::vec2(0.0f)) {};

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) noexcept :
			position(position),
			normal(normal),
			uv(uv) {};

	glm::vec3		position;
	glm::vec3		normal;
	glm::vec2		uv;
};

struct Texture {
	enum eType { NONE, DIFFUSE, SPECULAR };

	unsigned int	id;
	eType			type;
	std::string		path;

	static unsigned int TextureFromFile(const char *path, const std::string &directory);
};

class Mesh {
public:

	enum class eFlag { NONE = 0, TEXTURE = (1 << 0) };

	Mesh(std::vector<Vertex> vertice, std::vector<unsigned int> indice, std::vector<Texture> texture);
	Mesh(std::vector<Vertex> vertice, std::vector<unsigned int> indice);

	void				render(Shader &shader, GLenum typeOfDraw = GL_TRIANGLES) const;
	void				render(GLenum typeOfDraw = GL_TRIANGLES) const noexcept;
	unsigned int		getVAO() const;
	void				activeTexture(Shader &shader) const;
	std::vector<unsigned int> const	&getIndice() const;

	~Mesh();

private:

	eFlag						flag_;
	std::vector<Vertex>			vertice_;
	std::vector<unsigned int>	indice_;
	std::vector<Texture>		texture_;



	unsigned int				VAO;
	unsigned int				VBO;
	unsigned int				EBO;

	void	clean_();
	void 	setupMesh_();

	static bool				debug_;

	Mesh() = delete;
	Mesh(Mesh const &) = delete;
	Mesh &operator=(Mesh const &) = delete;
};
