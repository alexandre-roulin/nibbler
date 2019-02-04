#include "Transform.hpp"

Transform::Transform() :
		transform_(1.f),
		rotate_(1.f),
		center_(0),
		scaling_(1.f),
		rotation_(0.f),
		position_(0.f),
		speed_(1.f),
		interScaling_(1.f)
{}

Transform::Transform(Transform const &src) {
	*this = src;
}

Transform &Transform::operator=(Transform const &src) {
	if (this != &src) {
		transform_ = src.transform_;
		rotate_ = src.rotate_;
		center_ = src.center_;
		scaling_ = src.scaling_;
		rotation_ = src.rotation_;
		position_ = src.position_;
		speed_ = src.speed_;
		interScaling_ = src.interScaling_;
	}
	return (*this);
}

void	Transform::setInterScaling(float interScaling) {
	interScaling_ = interScaling;
}
void	Transform::setCenter(glm::vec3 const &center) {
	center_ = center;
}

void	Transform::translate(const glm::vec3 &axis, float deltaTime) {
	float velocity = speed_ * deltaTime;

	position_ += (axis * velocity);
}
void	Transform::rotate(glm::vec3 const &axis, float angle, float deltaTime) {
	float velocity = speed_ * deltaTime;

	rotate_ = glm::rotate(rotate_, (angle * velocity), axis);
}
void	Transform::scale(glm::vec3 const &axis, float deltaTime) {
	float velocity = speed_ * deltaTime;

	scaling_ += (axis * velocity);
}
void	Transform::scale(float scale, float deltaTime) {
	float velocity = speed_ * deltaTime;

	scaling_ += glm::vec3(scale) * velocity;
}

void	Transform::resetTransform() {
	transform_ = glm::mat4(1.0f);
	rotate_ = glm::mat4(1.0f);
	scaling_ = glm::vec3(1.f);
	position_ = glm::vec3(0.f);
}

void	Transform::updateTransform() {
	glm::mat4 scale(1.f);
	glm::mat4 toOrigin(1.f);

	scale = glm::scale(scale, (interScaling_ * scaling_));
	toOrigin = glm::translate(glm::mat4(1.f), -center_);

	transform_ = glm::translate(glm::mat4(1.f), position_)
				 * rotate_
				 * scale
				 * toOrigin;

}

glm::mat4	Transform::getTransform() const {
	return transform_;
}

glm::vec3	Transform::getPosition() const {
	return position_;
}

void Transform::setPosition(glm::vec3 const &pos) {
	position_ = pos;
}

