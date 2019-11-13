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
		"layout (location = 2) in vec3 Normal;"
		"out vec3 color;"
		"out vec3 normal;"
		"out vec4 fragment_position;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		"void main() {"
		"  gl_Position = proj * view * model * vec4(position, 1.0);"
		"  fragment_position = model * vec4(position, 1.0);"
		"  color = ourColor;"
		"  normal = normalize((model * vec4(Normal, 0.0)).xyz);"
		"}";

	const char* fragment_shader = 
		"#version 330 core\n"
		"in vec3 color;"
		"in vec3 normal;"
		"in vec4 fragment_position;"
		"out vec4 frag_color;"
		"uniform vec3 light_color;"
		"uniform vec3 light_position;"
		"void main() {"
		"  float light_radius = 50.0;"
		"  float far_comp = sqrt(max((light_radius - length(fragment_position.xyz - light_position)) / light_radius, 0.0));"
		"  float ambient_strengh = 0.1;"
		"  vec3 ambient_color = ambient_strengh * light_color;"
		"  vec3 light_direction = normalize(light_position - fragment_position.xyz);"
		"  vec3 diffuse = max(dot(normal, light_direction), 0.0) * far_comp * light_color;"
		"  vec3 res = (ambient_color + diffuse) * color;"
		"  if (res.x > 1.0) {"
		"    res.x = 1.0;"
		"  }"
		"  if (res.y > 1.0) {"
		"    res.y = 1.0;"
		"  }"
		"  if (res.z > 1.0) {"
		"    res.z = 1.0;"
		"  }"
		"  frag_color = vec4(res, 0.0);"
		"}";

	const char* light_shader =
		"#version 330 core\n"
		"out vec4 frag_color;"
		"uniform vec3 light_color;"
		"void main() {"
		"  frag_color = vec4(light_color, 0.0);"
		"}";

	const char* sphere_vertex =
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;"
		"void main() {"
		"  "
		"}";

	Shader shaderProgram(vertex_shader, fragment_shader, "model", "view", "proj"), 
		lightShaderProgram(vertex_shader, light_shader, "model", "view", "proj");


	shaderProgram.addOptionalUniform("light_position"); 
	shaderProgram.addOptionalUniform("light_color");

	lightShaderProgram.addOptionalUniform("light_color");

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

	std::vector<GLfloat> vertices_buffer;
	std::vector<glm::vec3> normales;
	std::cout << indices.size() << std::endl;
	std::cout << vertices.size() << std::endl;

	//generate normals and add them to vertex array
	for (int i = 0; i < indices.size() - 2; i += 3) {
		std::cout << i << std::endl;
		glm::vec3 vert1, vert2, vert3;
		vert1 = glm::vec3(vertices[indices[i] * 6], vertices[indices[i] * 6 + 1], vertices[indices[i] * 6 + 2]);

		vert2 = glm::vec3(vertices[indices[i + 1] * 6], vertices[indices[i + 1] * 6 + 1], vertices[indices[i + 1] * 6 + 2]);

		vert3 = glm::vec3(vertices[indices[i + 2] * 6], vertices[indices[i + 2] * 6 + 1], vertices[indices[i + 2] * 6 + 2]);

		glm::vec3 norm1 = glm::normalize(glm::cross((vert3 - vert1), (vert2 - vert1)));
		if (glm::length(vert1 + norm1) < glm::length(vert1 - norm1)) {
			normales.push_back(-norm1);
		}
		else {
			normales.push_back(norm1);
		}

		glm::vec3 norm2 = glm::normalize(glm::cross((vert1 - vert2), (vert3 - vert2)));
		if (glm::length(vert2 + norm2) < glm::length(vert2 - norm2)) {
			normales.push_back(-norm2);
		}
		else {
			normales.push_back(norm2);
		}

		glm::vec3 norm3 = glm::normalize(glm::cross((vert2 - vert3), (vert1 - vert3)));
		if (glm::length(vert3 + norm3) < glm::length(vert3 - norm3)) {
			normales.push_back(-norm3);
		}
		else {
			normales.push_back(norm3);
		}

		//first vertex
		//coords
		vertices_buffer.push_back(vert1.x);                         
		vertices_buffer.push_back(vert1.y);
		vertices_buffer.push_back(vert1.z);
		//colors
		vertices_buffer.push_back(vertices[indices[i] * 6 + 3]);    
		vertices_buffer.push_back(vertices[indices[i] * 6 + 4]);
		vertices_buffer.push_back(vertices[indices[i] * 6 + 5]);
		//normales
		vertices_buffer.push_back(normales[i].x);
		vertices_buffer.push_back(normales[i].y);
		vertices_buffer.push_back(normales[i].z);

		//second vertex
		vertices_buffer.push_back(vert2.x);
		vertices_buffer.push_back(vert2.y);
		vertices_buffer.push_back(vert2.z);
		vertices_buffer.push_back(vertices[indices[i + 1] * 6 + 3]);
		vertices_buffer.push_back(vertices[indices[i + 1] * 6 + 4]);
		vertices_buffer.push_back(vertices[indices[i + 1] * 6 + 5]);
		vertices_buffer.push_back(normales[i + 1].x);
		vertices_buffer.push_back(normales[i + 1].y);
		vertices_buffer.push_back(normales[i + 1].z);

		//third vertex
		vertices_buffer.push_back(vert3.x);
		vertices_buffer.push_back(vert3.y);
		vertices_buffer.push_back(vert3.z);
		vertices_buffer.push_back(vertices[indices[i + 2] * 6 + 3]);
		vertices_buffer.push_back(vertices[indices[i + 2] * 6 + 4]);
		vertices_buffer.push_back(vertices[indices[i + 2] * 6 + 5]);
		vertices_buffer.push_back(normales[i + 2].x);
		vertices_buffer.push_back(normales[i + 2].y);
		vertices_buffer.push_back(normales[i + 2].z);
	}
	std::cout << vertices_buffer.size() << std::endl;
	VAO vao;
	vao.addVertexBufferObject(vertices_buffer, { 3, 3, 3 }, 9, { 0, 3, 6 });
	//vao.addElementBufferObject(indices);
	glBindVertexArray(0);

	VAO light;
	light.addVertexBufferObject(vertices, { 3 }, 6, { 0 });
	light.addElementBufferObject(indices);
	glBindVertexArray(0);

	GLfloat time;
	GLfloat colR;
	GLfloat colG;
	GLfloat colB;
	

	glm::mat4 model, view, proj;
	glm::mat4 light_model;
	glm::vec3 light_position = glm::vec3((GLfloat)2.5f, (GLfloat)3.0f, (GLfloat)1.0f);

	//transform order: translate, rotate, scale

	Camera camera(6.0f, 0.01f);
	
	proj = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	model = glm::translate(model, glm::vec3(-0.7f, 0.2f, 0.0f));
	model = glm::rotate(model, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	light_model = glm::translate(light_model, light_position);
	light_model = glm::scale(light_model, glm::vec3(0.2f, 0.2f, 0.2f));
	std::cout << glm::length(light_position - glm::vec3(-0.7f, 0.2f, 0.0f)) << std::endl;
	
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);

	while (!glfwWindowShouldClose(win)) {

		glfwPollEvents();
		camera.move(keys[GLFW_KEY_LEFT_CONTROL], keys[GLFW_KEY_LEFT_SHIFT], keys[GLFW_KEY_S],
			keys[GLFW_KEY_W], keys[GLFW_KEY_A], keys[GLFW_KEY_D]);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (keys[GLFW_KEY_T]) {
			model = glm::translate(model, glm::vec3(0.01f, 0.01f, 0.0f));
		}
		else if (keys[GLFW_KEY_G]) {
			model = glm::translate(model, glm::vec3(-0.01f, -0.01f, 0.0f));
		}

		view = camera.getViewMatrix();
		time = glfwGetTime();
		colR = sin(time) / 4 + 0.25;
		colG = sin(time + 90) / 4 + 0.25;
		colB = sin(time - 90) / 4 + 0.25;
		//model = glm::rotate(model, sqrt(sqrt((GLfloat)time)) * 0.1f, glm::vec3(0.5f, 0.0f, 0.0f));
		
		shaderProgram.use(model, view, proj, std::vector<glm::vec3> {light_position, glm::vec3(1.0f, 1.0f, 1.0f)});
		vao.drawVx(36);

		lightShaderProgram.use(light_model, view, proj, std::vector<glm::vec3> {glm::vec3(1.0f, 1.0f, 1.0f)});
		light.drawEl(36);
		
		glfwSwapBuffers(win);
	}

	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}
