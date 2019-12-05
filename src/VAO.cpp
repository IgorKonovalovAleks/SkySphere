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

void VAO::addVertexBufferObject(const std::vector<GLfloat>& data, const std::vector <GLint>& counts,
	GLint len, const std::vector<GLint>& intendations) {
	GLuint vbo;
	glGenBuffers(1, &vbo);
	bind();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);
	
	for (int i = 0; i < counts.size(); i++) {
		glVertexAttribPointer(i, counts[i], GL_FLOAT, GL_FALSE, len * sizeof(GLfloat), (GLvoid*)(intendations[i] * sizeof(GLfloat)));
	}
	for (int i = 0; i < counts.size(); i++) {
		glEnableVertexAttribArray(i);
	}
	
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

void VAO::drawEl(GLsizei c) {
	bind();
	glDrawElements(GL_TRIANGLES, c, GL_UNSIGNED_INT, 0);

	unBind();

}

void VAO::drawVx(GLsizei c) {
	bind();
	glDrawArrays(GL_POINTS, 0, c);
	unBind();
}

void VAO::drawVxStrip(GLsizei c, GLsizei intendation)
{
	bind();
	glDrawArrays(GL_POINTS, intendation, c);
	unBind();
}

VAO::~VAO() {
	glDeleteBuffers(Vbuffers.size(), Vbuffers.data());
	glDeleteBuffers(Ebuffers.size(), Ebuffers.data());
	glDeleteVertexArrays(1, &mVao);
}
