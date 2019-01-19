#pragma  once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Model.hpp"
#include "ActModel.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Particle {
public:

	Particle(Model &model, unsigned int size);

	~Particle();

	void render(Shader &shader);
	void update();

	std::vector<ActModel>		transforms;

private:
	unsigned int	size_;
	Model			&model_;

	unsigned int				BOParticle_; //Commun a chaque Mesh VAO
	glm::mat4					*bufferTransform;

	void updateTransforms_();

	Particle() = delete;
	Particle(Particle const &) = delete;
	Particle &operator=(Particle const &) = delete;
};
