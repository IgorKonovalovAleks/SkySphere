#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &mVao);
}

void VAO::bind() {
	glBindVertexArray(mVao);
}

void VAO::unBind() {
	glBindVertexArray(0);
}

void VAO::addVertexBufferObject(const std::vector<GLfloat>& data) {
	GLuint vbo;
	glGenBuffers(1, &vbo);
	bind();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	unBind();
	Vbuffers.push_back(vbo);
}

void VAO::addElementBufferObject(const std::vector<GLuint>& data) {
	GLuint ebo;
	glGenBuffers(1, &ebo);
	bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * data.size(), data.data(), GL_STATIC_DRAW);
	
	unBind();
	Ebuffers.push_back(ebo);
}

void VAO::draw(GLsizei c) {
	bind();
	glDrawElements(GL_TRIANGLES, c, GL_UNSIGNED_INT, 0);

	unBind();

}

VAO::~VAO() {
	glDeleteBuffers(Vbuffers.size(), Vbuffers.data());
	glDeleteBuffers(Ebuffers.size(), Ebuffers.data());
	glDeleteVertexArrays(1, &mVao);
}
