#include <iostream>
#include<GL/glew.h>
#include<GL/GL.h>
#include <GLFW/glfw3.h>
#include"VAO.h"

const int WIDTH = 854;
const int HEIGHT = 500;

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
		"#version 150\n"
		"in vec3 position;"
		"void main() {"
		"  gl_Position = vec4(position.x, position.y, position.z, 1.0);"
		"}";

	const char* fragment_shader = 
		"#version 150\n"
		"out vec4 frag_colour;"
		"void main() {"
		"  frag_colour = vec4(0.0, 0.0, 1.0, 1.0);"
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

	VAO vao;
	vao.addVertexBufferObject(
		{ 0.5, -0.5, 0,
		-0.5, -0.5, 0,
		0, 0.5, 0 }
	);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(win)) {

		glfwPollEvents();
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		vao.draw(3);
		glfwSwapBuffers(win);
	}
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}
