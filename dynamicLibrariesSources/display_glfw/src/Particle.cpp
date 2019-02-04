
#include "Particle.hpp"
#include "Shader.hpp"
#include <iostream>

Particle::Particle(std::string const &path, unsigned int size) :
path_(path),
bPhysicsMovement_(false),
size_(size),
transforms(size),
model_(path),
physicsMovement_(size)
{
	bufferTransform.resize(size);
	updateTransforms_();

	glGenBuffers(1, &BOParticle_);
	glBindBuffer(GL_ARRAY_BUFFER, BOParticle_);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::mat4), bufferTransform.data(), GL_STREAM_DRAW);


	for (const auto &mesh : model_.getMeshes()) {
		unsigned int VAO = mesh.getVAO();
		glBindVertexArray(VAO);
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	for (auto &transform : transforms) {
		transform.assign(&model_);
		transform.resetTransform();
	}
}

Particle::~Particle() {
}

void Particle::setPhysicsMovement(bool b) {
	bPhysicsMovement_ = b;
}

void Particle::updateTransforms_() {
	unsigned int i = 0;
	for (const auto &trans : transforms) {
		bufferTransform[i] = trans.getTransform();
		i++;
	}
}

void Particle::updatePhysicsMovement(glm::vec3 target, float deltaTime) {

	unsigned int i = 0;
	for (auto &transform : transforms) {
		glm::vec3 dist;
		glm::vec3 acceleration;
		glm::vec3 position = transform.getPosition();

		dist = target - position;

		if (dist.x && dist.y) {
			acceleration = glm::normalize(dist);
			physicsMovement_[i].velocity += acceleration;
		}
		else
			physicsMovement_[i].velocity += glm::vec3(0.000001f);
		transform.translate(physicsMovement_[i].velocity, deltaTime);
		i++;
	}
}

void Particle::update() {
	updateTransforms_();

	glBindBuffer(GL_ARRAY_BUFFER, BOParticle_);
	glBufferData(GL_ARRAY_BUFFER, size_ * sizeof(glm::mat4), bufferTransform.data(), GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_ * sizeof(glm::mat4), bufferTransform.data());
}

void Particle::render(Shader &shader, GLenum typeOfDraw) {
	shader.activate();
	for (const auto &mesh : model_.getMeshes()) {
		mesh.activeTexture(shader);
		glBindVertexArray(mesh.getVAO());
		glDrawElementsInstanced(typeOfDraw, static_cast<GLsizei>(mesh.getIndice().size()), GL_UNSIGNED_INT, 0, size_);
	}
}