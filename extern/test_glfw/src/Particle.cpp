
#include "Particle.hpp"
#include "Shader.hpp"
#include <iostream>

Particle::Particle(Model &model, unsigned int size) :
size_(size),
model_(model),
transforms(size)
{
	bufferTransform = new glm::mat4[size];
	for (auto &transform : transforms) {
		transform.assign(&model);
	}
	updateTransforms_();

	glGenBuffers(1, &BOParticle_);
	glBindBuffer(GL_ARRAY_BUFFER, BOParticle_);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::mat4), bufferTransform, GL_STREAM_DRAW);


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
}

Particle::~Particle() {
	delete [] bufferTransform;
}

void Particle::updateTransforms_() {
	unsigned int i = 0;
	for (const auto &trans : transforms) {
		bufferTransform[i] = trans.getTransform();
		i++;
	}
}

void Particle::update() {
	updateTransforms_();

	glBindBuffer(GL_ARRAY_BUFFER, BOParticle_);
	glBufferData(GL_ARRAY_BUFFER, size_ * sizeof(glm::mat4), bufferTransform, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_ * sizeof(glm::mat4), bufferTransform);
}

void Particle::render(Shader &shader) {
	shader.activate();
	for (const auto &mesh : model_.getMeshes()) {
		mesh.activeTexture(shader);
		glBindVertexArray(mesh.getVAO());
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(mesh.getIndice().size()), GL_UNSIGNED_INT, 0, size_);
	}
}