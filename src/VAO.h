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
	std::vector<GLuint> Ebuffers;
	
public:
	VAO();
	VAO(const VAO&) = delete;
	void bind();
	void unBind();
	void addVertexBufferObject(const std::vector<GLfloat>& data);
	void addElementBufferObject(const std::vector<GLuint>& data);
	void draw(GLsizei c);
	~VAO();

};

