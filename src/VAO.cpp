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

void VAO::addVertexBufferObject(const std::vector<float>& data) {
	GLuint vbo;
	glGenBuffers(1, &vbo);
	bind();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
	glEnableVertexAttribArray(0);
	unBind();
	Vbuffers.push_back(vbo);
}

void VAO::draw(GLsizei c) {
	bind();
	glDrawArrays(GL_TRIANGLES, 0, c);

	unBind();

}

VAO::~VAO() {
	glDeleteBuffers(Vbuffers.size(), Vbuffers.data());
	glDeleteVertexArrays(1, &mVao);
}
