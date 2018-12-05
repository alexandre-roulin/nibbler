#pragma  once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Shader;

struct Vertex {

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
};

class Mesh {
public:

	enum class eFlag { NONE = 0, TEXTURE = (1 << 0) };

	Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> indice, std::vector<Texture> texture);
	Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> indice);

	void render(Shader &shader);
	void render() const noexcept;

	~Mesh();

private:
	eFlag						flag_;

	std::vector<Vertex>			vertex_;
	std::vector<unsigned int>	indice_;
	std::vector<Texture>		texture_;

	unsigned int				VAO;
	unsigned int				VBO;
	unsigned int				EBO;

	void	clean_();
	void 	setupMesh_();

	static bool				debug_;

	Mesh() = delete;
	Mesh(Mesh const &Mesh) = delete;
	Mesh &operator=(Mesh const &Mesh) = delete;
};
