#pragma once
#include <iostream>
#include <GL/glew.h>
class Shader
{
private:
	GLuint id;
public:
	Shader(const char* vertex_source, const char* fragment_source);
	GLuint getId();
	~Shader();
};

