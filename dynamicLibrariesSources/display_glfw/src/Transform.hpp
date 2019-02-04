#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <bitset>
#include <glad/glad.h>

class Transform {
public:

	Transform();
	virtual ~Transform() = default;
	Transform(Transform const &src);
	Transform &operator=(Transform const &src);

	void			resetTransform();
	glm::mat4		getTransform() const;
	glm::vec3		getPosition() const;

	void	setInterScaling(float interScaling);
	void	setCenter(glm::vec3 const &center);
	void 	setPosition(glm::vec3 const &pos);


	void	translate(glm::vec3 const &axis, float deltaTime = 1.f);
	void	rotate(glm::vec3 const &axis, float angle, float deltaTime = 1.f);
	void	scale(glm::vec3 const &axis, float deltaTime = 1.f);
	void	scale(float, float deltaTime = 1.f);

	void	updateTransform();

protected:
	glm::mat4			transform_;
	glm::mat4			rotate_;

	glm::vec3			center_;
	glm::vec3			scaling_;
	glm::vec3			rotation_;
	glm::vec3			position_;
	float				speed_;
	float				interScaling_;
};
