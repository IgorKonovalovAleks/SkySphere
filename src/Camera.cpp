#include "Camera.h"

Camera::Camera() {
	speed = 0.05f;
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, vecUp);
}

void Camera::move(bool up, bool down, bool left, bool right) {
	if (up and !down) {
		position += front * speed;
	}
	else if (down) {
		position -= front * speed;
	}

	if (right and !left) {
		position += glm::normalize(glm::cross(front, vecUp)) * speed;
	}
	else if (left) {
		position -= glm::normalize(glm::cross(front, vecUp)) * speed;
	}
}

