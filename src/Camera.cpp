#include "Camera.h"

Camera::Camera(GLfloat z, float s) {
	speed = s;
	position = glm::vec3(0.0f, 0.0f, z);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	calc.setRadius(z);
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), vecUp);
}

void Camera::move(bool forward, bool backward, bool up, bool down, bool left, bool right) {
	
	if (forward and !backward) {
		calc.changeRadius(speed);
		position = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
	}
	else if (backward) {
		calc.changeRadius(-speed);
		position = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
	}

	if (up and !down) {
		calc.changeVetrical(-speed);
		position = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
	}
	else if (down) {
		calc.changeVetrical(speed);
		position = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
	}

	if (right and !left) {
		calc.changeHorizontal(speed);
		position = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
	}
	else if (left) {
		calc.changeHorizontal(-speed);
		position = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
	}
}

