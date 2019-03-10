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
	~Camera() = default;

	Camera(Camera const &camera);
	Camera &operator=(Camera const &camera);

	void update();
	void setPosition(glm::vec3 const &pos);
	void setFront(glm::vec3 const &front);
	void setUp(glm::vec3 const &up);

	void processPosition(Camera::Movement direction, float deltaTime = 1.f);
	void processPosition(glm::vec3 const &, float deltaTime = 1.f);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const;


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

	void updateCameraVectors_();
};
