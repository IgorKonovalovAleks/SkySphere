#pragma once
#include<GL/glew.h>
#include<gl/wglew.h>
#include<GL/GL.h>
#include<iostream>
#include<vector>

class VAO
{
private:
	GLuint mVao;
	std::vector<GLuint> Vbuffers;
	
public:
	VAO();
	VAO(const VAO&) = delete;
	void bind();
	void unBind();
	void addVertexBufferObject(const std::vector<float>& data);
	void draw(GLsizei c);
	~VAO();

};

