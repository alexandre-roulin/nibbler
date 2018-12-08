#include "Camera.hpp"

Camera::Camera() :
	position_(glm::vec3(0.0f, 0.0f, 0.0f)),
	front_(glm::vec3(0.0f, 0.0f, -1.0f)),
	up_(glm::vec3(0.0f, 1.0f, 0.0f)),
	worldUp_(up_),
	yaw_(-90.0f),
	pitch_(0.0f),
	speed_(2.5f),
	sensitivity_(0.1f),
	zoom_(45.0f) {
	updateCameraVectors_();
}

void 		Camera::update() {
}

void		Camera::processPosition(Camera::Movement direction, float deltaTime) {
	float velocity = speed_ * deltaTime;

	if (direction == Camera::Movement::FORWARD)
		position_ += front_ * velocity;
	else if (direction == Camera::Movement::BACKWARD)
		position_ -= front_ * velocity;
	else if (direction == Camera::Movement::LEFT)
		position_ -= right_ * velocity;
	else if (direction == Camera::Movement::RIGHT)
		position_ += right_ * velocity;
	updateCameraVectors_();
}

void		Camera::processPosition(glm::vec3 const &move, float deltaTime) {
	float velocity = speed_ * deltaTime;

	position_ += front_ * (move * velocity);
	updateCameraVectors_();
}


void		Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= sensitivity_;
	yoffset *= sensitivity_;

	yaw_   += xoffset;
	pitch_ += yoffset;

	if (constrainPitch) {
		if (pitch_ > 89.0f)
			pitch_ = 89.0f;
		if (pitch_ < -89.0f)
			pitch_ = -89.0f;
	}
	updateCameraVectors_();
}

glm::mat4	Camera::getViewMatrix() const {
	return glm::lookAt(position_, position_ + front_, up_);
}

void		Camera::updateCameraVectors_() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

	front_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(front_, worldUp_));
	up_    = glm::normalize(glm::cross(right_, front_));
}
