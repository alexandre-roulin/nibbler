#pragma  once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <vector>

class Material {
public:

	Material() = delete;
	Material(std::string const &name = std::string("tMaterial"), float shininess = 76.8f,
				glm::vec3 const &ambient = {0.0215f, 0.1745f, 0.0215f},
			 	glm::vec3 const &diffuse = {0.0756f, 0.61423f, 0.07568f},
			 	glm::vec3 const &specular = {0.633f, 0.7278f, 0.633f});
	~Material();
	Material(Material const &) = delete;
	Material &operator=(Material const &) = delete;
	
	void 	putMaterialToShader(class Shader const &shader) const;
	static void unsetMaterial(class Shader const &shader);

private:
	std::string		name_;

	float			shininess_;
	glm::vec3		ambient_;
	glm::vec3		diffuse_;
	glm::vec3		specular_;
};