#pragma  once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Model.hpp"
#include "ActModel.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct PhysicsMovement {
	glm::vec3	velocity;
	glm::vec3	direction;

	PhysicsMovement() :
			velocity(glm::vec3(0.f)),
			direction(glm::vec3(0.f))
	{}
};

class Particle {
public:

	Particle(Model &model, unsigned int size);

	~Particle();

	void render(Shader &shader);
	void update();
	void updatePhysicsMovement(glm::vec3 target, float deltaTime = 0.16f);

	void setPhysicsMovement(bool b);

	std::vector<ActModel>			transforms;

private:
	bool			bPhysicsMovement_;
	unsigned int	size_;
	Model			&model_;

	unsigned int				BOParticle_; //Commun a chaque Mesh VAO
	glm::mat4					*bufferTransform;
	std::vector<PhysicsMovement>	physicsMovement_;

	void updateTransforms_();

	Particle() = delete;
	Particle(Particle const &) = delete;
	Particle &operator=(Particle const &) = delete;
};