#pragma once
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class Shader
{
private:
	GLuint id;
	GLuint model;
	GLuint view;
	GLuint projection;
	std::vector<GLuint> optional;
public:
	Shader();
	Shader(const char* vertex_source, const char* fragment_source);
	Shader(const char* vertex_source, const char* fragment_source, const char* model_name, const char* view_name, const char* projection_name);
	Shader(const char* vertex_source, const char* geo_source, const char* fragment_source, const char* model_name, const char* view_name, const char* projection_name);
	void use();
	void use(glm::mat4& m, glm::mat4& v, glm::mat4& p);
	void use(glm::mat4& m, glm::mat4& v, glm::mat4& p, const std::vector<glm::vec3>& optional_values);
	void addOptionalUniform(const char* name); 
	GLuint getId();
	~Shader();
};

