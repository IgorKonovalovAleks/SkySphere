#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SphereCalculator.h"
#pragma once
class Camera
{
private:
	const glm::vec3 vecUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 position;
	glm::vec3 front;
	GLfloat speed;
	SphereCalculator calc;
public:
	Camera(GLfloat z, float s);
	glm::mat4 getViewMatrix();
	void move(bool forward, bool backward, bool up, bool down, bool left, bool right);
};
