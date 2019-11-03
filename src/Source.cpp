#include <iostream>
#include "VAO.h"
#include <GL/GL.h>
#include "Shader.h"
#include "Camera.h"

const int WIDTH = 800;
const int HEIGHT = 600;
bool keys[1024];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

int main() {

	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR" << std::endl;
		return -1;
	}

	SphereCalculator calc;

	GLFWwindow* win = glfwCreateWindow(WIDTH, HEIGHT, "Engine", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glewExperimental = true;
	glfwSetKeyCallback(win, key_callback);
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW INIT ERROR" << std::endl;
		return -1;
	}
	int width, height;
	glfwGetFramebufferSize(win, &width, &height);

	glViewport(0, 0, width, height);

	const char* vertex_shader = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;"
		"layout (location = 1) in vec3 ourColor;"
		"out vec3 color;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		"void main() {"
		"  gl_Position = proj * view * model * vec4(position.x, position.y, position.z, 1.0);"
		"  color = ourColor;"
		"}";

	const char* fragment_shader = 
		"#version 330 core\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"uniform vec4 mColor;"
		"void main() {"
		"  frag_color = color + mColor;"
		"}";

	const char* light_shader =
		"#version 330 core\n"
		"out vec4 frag_color;"
		"void main() {"
		"  frag_color = vec4(1.0, 1.0, 1.0, 1.0);"
		"}";

	Shader shaderProgram(vertex_shader, fragment_shader), 
		lightShaderProgram(vertex_shader, light_shader);


	std::vector<GLfloat> vertices = {
	//forward
	 0.5f,  0.5f,  0.5f, 0.5f, 0.0f, 0.0f, /*up-right*/    
	 0.5f, -0.5f,  0.5f, 0.0f, 0.5f, 0.0f, /*down-right*/ 
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.5f, /*down-left*/  
	-0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.0f, /*up-left*/    
	//backward
	 0.5f,  0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,
	-0.5f,  0.5f, -0.5f, 0.5f, 0.5f, 0.0f
	};

	std::vector<GLuint> indices = {  
		0, 3, 4,
		3, 4, 7,
		0, 1, 4,
		4, 5, 1,
		0, 1, 3,
		1, 2, 3,
		4, 7, 5,
		5, 7, 6,
		1, 2, 5,
		5, 2, 6,
		2, 6, 3,
		3, 6, 7
	};

	VAO vao;
	vao.addVertexBufferObject(vertices, { 3, 3 }, 6, { 0, 3 });
	vao.addElementBufferObject(indices);
	glBindVertexArray(0);

	VAO light;
	light.addVertexBufferObject(vertices, { 3 }, 6, { 0 });
	light.addElementBufferObject(indices);
	glBindVertexArray(0);

	GLfloat time;
	GLfloat colR;
	GLfloat colG;
	GLfloat colB;
	GLint form_position;
	GLuint model_position; 
	GLuint view_position;
	GLuint proj_position;

	GLuint light_model_position, light_view_position, light_proj_position;

	model_position = glGetUniformLocation(shaderProgram.getId(), "model");
	view_position = glGetUniformLocation(shaderProgram.getId(), "view");
	proj_position = glGetUniformLocation(shaderProgram.getId(), "proj");
	form_position = glGetUniformLocation(shaderProgram.getId(), "mColor");

	light_model_position = glGetUniformLocation(lightShaderProgram.getId(), "model");
	light_view_position = glGetUniformLocation(lightShaderProgram.getId(), "view");
	light_proj_position = glGetUniformLocation(lightShaderProgram.getId(), "proj");

	glm::mat4 model, view, proj;
	glm::mat4 light_model;

	//transform order: translate, rotate, scale

	Camera camera(6.0f, 0.01f);
	
	proj = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	model = glm::translate(model, glm::vec3(-0.7f, 0.2f, 0.0f));
	model = glm::rotate(model, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	light_model = glm::translate(light_model, glm::vec3(2.5f, 3.0f, 1.0f));
	light_model = glm::scale(light_model, glm::vec3(0.3f, 0.3f, 0.3f));
	
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(win)) {

		glfwPollEvents();
		camera.move(keys[GLFW_KEY_LEFT_CONTROL], keys[GLFW_KEY_LEFT_SHIFT], keys[GLFW_KEY_S],
			keys[GLFW_KEY_W], keys[GLFW_KEY_A], keys[GLFW_KEY_D]);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		view = camera.getViewMatrix();
		time = glfwGetTime();
		colR = sin(time) / 4 + 0.25;
		colG = sin(time + 90) / 4 + 0.25;
		colB = sin(time - 90) / 4 + 0.25;
		model = glm::rotate(model, sqrt(sqrt((GLfloat)time)) * 0.1f, glm::vec3(0.5f, 1.0f, 0.0f));
		
		glUseProgram(shaderProgram.getId());

		glUniform4f(form_position, colR, colG, colB, 1.0f);
		
		glUniformMatrix4fv(model_position, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(view_position, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(proj_position, 1, GL_FALSE, glm::value_ptr(proj));

		vao.draw(36);

		glUseProgram(lightShaderProgram.getId());

		glUniformMatrix4fv(light_model_position, 1, GL_FALSE, glm::value_ptr(light_model));
		glUniformMatrix4fv(light_view_position, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(light_proj_position, 1, GL_FALSE, glm::value_ptr(proj));

		light.draw(36);
		
		glfwSwapBuffers(win);
	}

	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}
