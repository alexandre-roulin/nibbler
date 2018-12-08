#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera {
public:
	enum Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	Camera();
	~Camera() {};


	void update();
	void processPosition(Camera::Movement direction, float deltaTime);
	void processPosition(glm::vec3 const &, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	glm::mat4 getViewMatrix() const;


private:
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 worldUp_;
	float yaw_;
	float pitch_;
	float speed_;
	float sensitivity_;
	float zoom_;

	Camera(Camera const &) = delete;
	Camera &operator=(Camera const &) = delete;

	void updateCameraVectors_();
};
