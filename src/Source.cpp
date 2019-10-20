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
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;"
		"void main() {"
		"  gl_Position = vec4(position.x, position.y, position.z, 1.0);"
		"}";

	const char* fragment_shader = 
		"#version 330 core\n"
		"out vec4 frag_color;"
		"uniform vec4 mColor;"
		"void main() {"
		"  frag_color = mColor;"
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
	 0.5f,  0.5f, 0.0f, 
	 0.5f, -0.5f, 0.0f, 
	-0.5f, -0.5f, 0.0f, 
	-0.5f,  0.5f, 0.0f  
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

	while (!glfwWindowShouldClose(win)) {

		glfwPollEvents();

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);

		time = glfwGetTime();
		colR = sin(time) / 2 + 0.5;
		colG = sin(time + 45) / 2 + 0.5;
		colB = sin(time + 90) / 2 + 0.5;
		form_position = glGetUniformLocation(shaderProgram, "mColor");
		glUniform4f(form_position, colR, colG, colB, 1.0f);

		vao.draw(6);
		glfwSwapBuffers(win);
	}
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}
