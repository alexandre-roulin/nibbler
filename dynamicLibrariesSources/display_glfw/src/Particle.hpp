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

	Particle() = delete;
	Particle(std::string const &, unsigned int size);
	~Particle();
	Particle(Particle const &) = delete;
	Particle &operator=(Particle const &) = delete;

	void render(Shader &shader, GLenum typeOfDraw = GL_TRIANGLES);
	void update();
	void updatePhysicsMovement(glm::vec3 target, float deltaTime = 0.16f);

	void setPhysicsMovement(bool b);

	std::vector<ActModel>			transforms;

private:
	std::string		path_;
	bool			bPhysicsMovement_;
	unsigned int	size_;
	Model			model_;

	unsigned int					BOParticle_; //Commun a chaque Mesh VAO
	std::vector<glm::mat4>			bufferTransform;
	std::vector<PhysicsMovement>	physicsMovement_;

	void updateTransforms_();
};