
#include "Light.hpp"
#include "Shader.hpp"
#include <iostream>

Light::Light(Model const *model, glm::vec3 const &position, float intensity) :
ActModel(model),
intensity_(intensity) {
	translate(position);
}
Light::Light(glm::vec3 const &position, float intensity) :
ActModel(),
intensity_(intensity)  {
	translate(position);
}

Light::~Light() {
}

void 		Light::putLightToShader(Shader const &shader) const {
	shader.setFloat("uLight.intensity", intensity_);
	shader.setVec3("uLight.position", getPosition());
}