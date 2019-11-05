#pragma once
#include<GL/glew.h>
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
	void addVertexBufferObject(const std::vector<GLfloat>& data, const std::vector<GLint>& counts,
		GLint len, const std::vector<GLint>& intendations);
	void addElementBufferObject(const std::vector<GLuint>& data);
	void drawEl(GLsizei c);
	void drawVx(GLsizei c);
	~VAO();

};

