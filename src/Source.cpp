#include <iostream>
#include "VAO.h"
#include <GL/GL.h>
#include "SphereElements.h"
#include "Camera.h"


const int WIDTH = 800;
const int HEIGHT = 600;
bool keys[1024];

SphereCalculator calc;
std::vector<GLfloat> sphere_buffer;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void initSphere() {
	calc.setRadius(1.0f);
	glm::vec3 pos;
	for (int hangle = 0; hangle < 40; hangle++) {
		for (int vangle = 0; vangle < 40; vangle++) {
			pos = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
			sphere_buffer.push_back(pos.x);
			sphere_buffer.push_back(pos.y);
			sphere_buffer.push_back(pos.z);
			calc.changeHorizontal(glm::radians(9.0f));

			pos = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
			sphere_buffer.push_back(pos.x);
			sphere_buffer.push_back(pos.y);
			sphere_buffer.push_back(pos.z);
			calc.changeVetrical(glm::radians(9.0f));

			pos = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
			sphere_buffer.push_back(pos.x);
			sphere_buffer.push_back(pos.y);
			sphere_buffer.push_back(pos.z);

			calc.changeHorizontal(-glm::radians(9.0f));
			pos = glm::vec3(calc.getX(), calc.getY(), calc.getZ());
			sphere_buffer.push_back(pos.x);
			sphere_buffer.push_back(pos.y);
			sphere_buffer.push_back(pos.z);

		}

		calc.changeHorizontal(glm::radians(9.0f));
	}
}

int main() {

	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR" << std::endl;
		return -1;
	}

	initSphere();

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

	const char* vertex =
		"#version 330 core\n"
		"layout (location = 0) in vec3 sph_coords;"
		"layout (location = 1) in vec3 col;"

		"out VS_OUT{"
		"  vec3 sphere_coords;"
		"  mat4 model;"
		"  mat4 view;"
		"  mat4 proj;"
		"  vec3 color;"
		"  vec4 frag_pos;"
		"} vs_out;"

		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"

		"void main() {"
		"  vs_out.sphere_coords = sph_coords;"
		"  vs_out.color = col;"
		"  gl_Position = proj * view * model * vec4(0.0, 0.0, 0.0, 1.0);"
		"  vs_out.frag_pos = model * vec4(0.0, 0.0, 0.0, 1.0);"
		"  vs_out.model = model;"
		"  vs_out.view = view;"
		"  vs_out.proj = proj;"
		"}";

	const char* geo =
		"#version 330 core\n"
		"layout (points) in;"
		"layout (line_strip, max_vertices = 81) out;"

		"in VS_OUT {"
		"  vec3 sphere_coords;"
		"  mat4 model;"
		"  mat4 view;"
		"  mat4 proj;"
		"  vec3 color;"
		"  vec4 frag_pos;"
		"} gs_in[];"

		"out vec3 color;"
		"out vec4 fragment_position;"

		"void main() {"
		"  color = gs_in[0].color;"
		"  float delta_h = radians(4.5);"
		"  float d = gs_in[0].sphere_coords.x;"
		"  float r = 1.0;"
		"  float h_angle = 0.0, max_delta = gs_in[0].sphere_coords.z;"
		"  float a = gs_in[0].sphere_coords.y, v_angle = d;"
		"  for(int i = 0; i < 81; i++) {"
		"    fragment_position = vec4(r * cos(v_angle) * sin(h_angle), r * sin(v_angle), r * cos(v_angle) * cos(h_angle), 1.0);"
		"    gl_Position = gs_in[0].proj * gs_in[0].view * gs_in[0].model * fragment_position;"
		"    EmitVertex();"
		"    h_angle += delta_h;"
		"  }"
		
		"  EndPrimitive();"
		"}";

	const char* fragment =
		"#version 330 core\n"
		"in vec4 fragment_position;"
		"in vec3 color;"

		"out vec4 frag_color;"
		"uniform vec3 light_color;"
		"uniform vec3 light_position;"

		"void main() {"
		"  float light_radius = 5.0;"
		"  float far_comp = sqrt(max((light_radius - length(fragment_position.xyz - light_position)) / light_radius, 0.0));"
		"  float ambient_strengh = 0.1;"
		"  vec3 ambient_color = ambient_strengh * light_color;"
		"  vec3 light_direction = normalize(light_position - fragment_position.xyz);"
		"  vec3 diffuse = max(dot(normalize(fragment_position.xyz), light_direction), 0.0) * far_comp * light_color;"
		"  vec3 res = (ambient_color + diffuse) * vec3(1.0, 1.0, 1.0);"
		"  if (res.x > 1.0) {"
		"    res.x = 1.0;"
		"  }"
		"  if (res.y > 1.0) {"
		"    res.y = 1.0;"
		"  }"
		"  if (res.z > 1.0) {"
		"    res.z = 1.0;"
		"  }"
		"  frag_color = vec4(color, 1.0);"
		"}";

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
		"  frag_color = vec4(res, 1.0);"
		"}";

	const char* light_shader =
		"#version 330 core\n"
		"out vec4 frag_color;"
		"uniform vec3 light_color;"
		"void main() {"
		"  frag_color = vec4(light_color, 1.0);"
		"}";

	const char* sphere_vertex =
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;"
		"out vec4 frg_ps;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		"void main() {"
		"  gl_Position = proj * view * model * vec4(position, 1.0);"
		"  frg_ps = model * vec4(position, 1.0);"
		"}";

	const char* geometry =
		"#version 330 core\n"
		"layout(points) in;"
		"layout(points, max_vertices = 1) out;"
		
		"in vec4 frg_ps[];"
		"out vec4 fragment_position;"

		"void main() {"
		"	gl_Position = gl_in[0].gl_Position;"
		"   fragment_position = frg_ps[0];"
		"	EmitVertex();"
		"	EndPrimitive();"
		"}  ";

	const char* sphere_fragment =
		"#version 330 core\n"
		"in vec4 fragment_position;"
		"out vec4 frag_color;"
		"uniform vec3 Slight_color;"
		"uniform vec3 Slight_position;"
		"void main() {"
		"  float light_radius = 5.0;"
		"  float far_comp = sqrt(max((light_radius - length(fragment_position.xyz - Slight_position)) / light_radius, 0.0));"
		"  float ambient_strengh = 0.1;"
		"  vec3 ambient_color = ambient_strengh * Slight_color;"
		"  vec3 light_direction = normalize(Slight_position - fragment_position.xyz);"
		"  vec3 diffuse = max(dot(normalize(fragment_position.xyz), light_direction), 0.0) * far_comp * Slight_color;"
//		"  vec3 diffuse = far_comp * Slight_color;"
		"  vec3 res = (ambient_color + diffuse) * vec3(1.0, 1.0, 1.0);"
		"  if (res.x > 1.0) {"
		"    res.x = 1.0;"
		"  }"
		"  if (res.y > 1.0) {"
		"    res.y = 1.0;"
		"  }"
		"  if (res.z > 1.0) {"
		"    res.z = 1.0;"
		"  }"
		"  frag_color = vec4(1.0);"
		"}";

	Shader shaderProgram(vertex_shader, fragment_shader, "model", "view", "proj"),
		lightShaderProgram(vertex_shader, light_shader, "model", "view", "proj"),
		sphereShaderProgram(sphere_vertex, geometry, sphere_fragment, "model", "view", "proj");


	shaderProgram.addOptionalUniform("light_position"); 
	shaderProgram.addOptionalUniform("light_color");

	sphereShaderProgram.addOptionalUniform("Slight_position");
	sphereShaderProgram.addOptionalUniform("Slight_color");

	lightShaderProgram.addOptionalUniform("light_color");

	std::vector<GLfloat> vertices = {
	//forward
	 0.3f,  0.3f,  0.3f, 0.3f, 0.0f, 0.0f, /*up-right*/    
	 0.3f, -0.3f,  0.3f, 0.0f, 0.3f, 0.0f, /*down-right*/ 
	-0.3f, -0.3f,  0.3f, 0.0f, 0.0f, 0.3f, /*down-left*/  
	-0.3f,  0.3f,  0.3f, 0.3f, 0.3f, 0.0f, /*up-left*/    
	//backward
	 0.3f,  0.3f, -0.3f, 0.3f, 0.0f, 0.0f,
	 0.3f, -0.3f, -0.3f, 0.0f, 0.3f, 0.0f,
	-0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 0.3f,
	-0.3f,  0.3f, -0.3f, 0.3f, 0.3f, 0.0f
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

	//generate normals and add them to vertex array
	for (int i = 0; i < indices.size() - 2; i += 3) {
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

	VAO vao;
	vao.addVertexBufferObject(vertices_buffer, { 3, 3, 3 }, 9, { 0, 3, 6 });
	//vao.addElementBufferObject(indices);
	glBindVertexArray(0);

	VAO light;
	light.addVertexBufferObject(vertices, { 3 }, 6, { 0 });
	light.addElementBufferObject(indices);
	glBindVertexArray(0);

	VAO sphere;
	sphere.addVertexBufferObject(sphere_buffer, { 3 }, 3, { 0 });
	glBindVertexArray(0);

	const std::vector<GLfloat> data =
		//d, horizontal, vertical, color
	{ glm::radians(0.0f), 0.0, 0.0, 1.0, 0.0, 0.0,                    //flats
	  glm::radians(6.78f), 0.0, 0.0, 0.0, 1.0, 0.0,
	  glm::radians(-34.36f), 0.0, 0.0, 0.0, 0.0, 1.0,
	  glm::radians(76.426f), 0.0, glm::radians(45.0f), 1.0, 0.0, 1.0,
	  glm::radians(0.0f), 0.0, glm::radians(23.156f), 1.0, 1.0, 1.0

	};

	glm::mat4 rot_ecl, rot_second, rot_third, rot_fourth, rot_hor;
	rot_ecl = glm::rotate(rot_ecl, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(rot_ecl, 23.156f, glm::vec3(0.0f, 0.0f, 1.0f));
	rot_second = glm::rotate(rot_second, 10.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(rot_second, 30.986f, glm::vec3(0.0f, 0.0f, 1.0f));
	rot_third = glm::rotate(rot_third, -10.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(rot_third, -78.62f, glm::vec3(0.0f, 0.0f, 1.0f));
	rot_fourth = glm::rotate(rot_fourth, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(rot_fourth, 33.33f, glm::vec3(0.0f, 0.0f, 1.0f));
	rot_hor = glm::rotate(rot_hor, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(rot_hor, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));



	Shader flat_shader(vertex, geo, fragment, "model", "view", "proj");
	flat_shader.addOptionalUniform("light_position");
	flat_shader.addOptionalUniform("light_color");
	VAO sphere_data;
	sphere_data.addVertexBufferObject(data, { 3, 3 }, 6, { 0, 3 });

	SphereElements sph;
	glBindVertexArray(0);

	glm::mat4 model, view, proj;
	glm::mat4 light_model;
	calc.setRadius(3.0f);
	calc.setHorizontal(glm::radians(90.0f));
	calc.setVetrical(glm::radians(45.0f));
	glm::vec3 light_position = glm::vec3((GLfloat)calc.getX(), (GLfloat)calc.getY(), (GLfloat)calc.getZ());

	//transform order: translate, rotate, scale

	Camera camera(6.0f, 0.03f);

	proj = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	model = glm::translate(model, glm::vec3(-0.7f, 0.2f, 0.0f));
	model = glm::rotate(model, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	light_model = glm::translate(light_model, light_position);
	light_model = glm::scale(light_model, glm::vec3(0.2f, 0.2f, 0.2f));
	std::cout << glm::length(light_position - glm::vec3(-0.7f, 0.2f, 0.0f)) << std::endl;
	
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 empty;

	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	GLfloat time;
	GLfloat colR;
	GLfloat colG;
	GLfloat colB;
	GLfloat delta_time = 1.0 / 60.0;
	GLfloat cur_delta = 0.0f, last_time = glfwGetTime();

	while (!glfwWindowShouldClose(win)) {

		time = glfwGetTime();
		cur_delta += time - last_time;
		last_time = time;
		if (cur_delta >= delta_time) {



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
			colR = sin(time) / 4 + 0.25;
			colG = sin(time + 90) / 4 + 0.25;
			colB = sin(time - 90) / 4 + 0.25;

			//shaderProgram.use(model, view, proj, std::vector<glm::vec3> {light_position, glm::vec3(1.0f, 1.0f, 1.0f)});
			//vao.drawVx(36);

			lightShaderProgram.use(light_model, view, proj, { glm::vec3(1.0f, 1.0f, 1.0f) });
			light.drawEl(36);

			sphereShaderProgram.use(empty, view, proj, { light_position, glm::vec3(1.0f, 1.0f, 1.0f) });
			sphere.drawVx((GLsizei)(sphere_buffer.size() / 3));

			//sph.draw(empty, view, proj, light_position, glm::vec3(1.0f, 1.0f, 1.0f));
			flat_shader.use(rot_ecl, view, proj, std::vector<glm::vec3> { light_position, glm::vec3(1.0f, 1.0f, 1.0f) });
			sphere_data.drawVxStrip(1, 0);

			flat_shader.use(rot_second, view, proj, std::vector<glm::vec3> { light_position, glm::vec3(1.0f, 1.0f, 1.0f) });
			sphere_data.drawVxStrip(1, 1);

			flat_shader.use(rot_third, view, proj, std::vector<glm::vec3> { light_position, glm::vec3(1.0f, 1.0f, 1.0f) });
			sphere_data.drawVxStrip(1, 2);

			flat_shader.use(rot_fourth, view, proj, std::vector<glm::vec3> { light_position, glm::vec3(1.0f, 1.0f, 1.0f) });
			sphere_data.drawVxStrip(1, 3);

			flat_shader.use(rot_hor, view, proj, std::vector<glm::vec3> { light_position, glm::vec3(1.0f, 1.0f, 1.0f) });
			sphere_data.drawVxStrip(1, 4);

			glfwSwapBuffers(win);

			cur_delta = 0.0f;
		}
	}

	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}
