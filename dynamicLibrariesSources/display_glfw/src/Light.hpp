#pragma  once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <vector>
#include "ActModel.hpp"

class Light : public ActModel {
public:

	Light() = delete;
	Light(Model const *model, glm::vec3 const &position = glm::vec3(0.f), float intensity = 1.f);
	Light(glm::vec3 const &position = glm::vec3(0.f), float intensity = 1.f);
	~Light() override;
	Light(Light const &) = delete;
	Light &operator=(Light const &) = delete;

	void 	putLightToShader(class Shader const &shader) const;

private:
	std::string		name_;
	float			intensity_;
};