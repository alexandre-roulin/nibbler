
#include "Material.hpp"
#include "Shader.hpp"
#include <iostream>

Material::Material(std::string const &name, float shininess,
				   glm::vec3 const &ambient,
				   glm::vec3 const &diffuse, glm::vec3 const &specular) :
		name_(name),
		shininess_(shininess),
		ambient_(ambient),
		diffuse_(diffuse),
		specular_(specular)

{
}

Material::~Material() {
}

void 		Material::putMaterialToShader(Shader const &shader) const {
	shader.setFloat("uMaterial.shininess", shininess_);
	shader.setVec3("uMaterial.diffuse", diffuse_);
	shader.setVec3("uMaterial.ambient", ambient_);
	shader.setVec3("uMaterial.specular", specular_);
}