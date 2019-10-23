#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VAO.h"

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {

	

	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR" << std::endl;
		return -1;
	}
	GLFWwindow* win = glfwCreateWindow(WIDTH, HEIGHT, "Engine", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glewExperimental = true;
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

	GLuint vertShader;
	GLuint fragShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertShader, 1, &vertex_shader, NULL);
	glCompileShader(vertShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glShaderSource(fragShader, 1, &fragment_shader, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);


	std::vector<GLfloat> vertices = {
	 0.5f,  0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,
	-0.5f,  0.5f, 0.0f,  0.5f, 0.5f, 0.0f
	};
	std::vector<GLuint> indices = {  
		0, 1, 3,   
		1, 2, 3    
	};

	VAO vao;
	vao.addVertexBufferObject(vertices);
	vao.addElementBufferObject(indices);
	glBindVertexArray(0);

	GLfloat time;
	GLfloat colR;
	GLfloat colG;
	GLfloat colB;
	GLint form_position;
	GLuint model_position; 
	GLuint view_position;
	GLuint proj_position;

	model_position = glGetUniformLocation(shaderProgram, "model");
	view_position = glGetUniformLocation(shaderProgram, "view");
	proj_position = glGetUniformLocation(shaderProgram, "proj");
	form_position = glGetUniformLocation(shaderProgram, "mColor");

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;

	model = glm::rotate(model, -70.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	proj = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(win)) {

		glfwPollEvents();

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);

		time = glfwGetTime();
		colR = sin(time) / 4 + 0.25;
		colG = sin(time + 90) / 4 + 0.25;
		colB = sin(time - 90) / 4 + 0.25;
		
		glUniform4f(form_position, colR, colG, colB, 1.0f);
		
		glUniformMatrix4fv(model_position, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(view_position, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(proj_position, 1, GL_FALSE, glm::value_ptr(proj));

		vao.draw(6);
		glfwSwapBuffers(win);
	}
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}
