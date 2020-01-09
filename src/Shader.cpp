#include "Shader.h"

Shader::Shader()
{
}

Shader::Shader(const char* vertex_source, const char* fragment_source)
{
	GLuint vertShader, fragShader;
	
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertShader, 1, &vertex_source, NULL);
	glCompileShader(vertShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glShaderSource(fragShader, 1, &fragment_source, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vertShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	model = 0;
	view = 0;
	projection = 0;

}

Shader::Shader(const char* vertex_source, const char* fragment_source, const char* model_name, const char* view_name, const char* projection_name)
{
	GLuint vertShader, fragShader;

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertShader, 1, &vertex_source, NULL);
	glCompileShader(vertShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glShaderSource(fragShader, 1, &fragment_source, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vertShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	model = glGetUniformLocation(id, model_name);
	view = glGetUniformLocation(id, view_name);
	projection = glGetUniformLocation(id, projection_name);


	std::cout << model << std::endl << view << std::endl << projection << std::endl;

}

Shader::Shader(const char* vertex_source, const char* geo_source, const char* fragment_source, const char* model_name, const char* view_name, const char* projection_name) {
	
	GLuint vertShader, geoShader, fragShader;

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	geoShader = glCreateShader(GL_GEOMETRY_SHADER);

	glShaderSource(vertShader, 1, &vertex_source, NULL);
	glCompileShader(vertShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glShaderSource(geoShader, 1, &geo_source, NULL);
	glCompileShader(geoShader);
	glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(geoShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glShaderSource(fragShader, 1, &fragment_source, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vertShader);
	glAttachShader(id, geoShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertShader);
	glDeleteShader(geoShader);
	glDeleteShader(fragShader);

	model = glGetUniformLocation(id, model_name);
	view = glGetUniformLocation(id, view_name);
	projection = glGetUniformLocation(id, projection_name);
	std::cout << model << " " << view << " " << projection << std::endl;
}

void Shader::use() {
	glUseProgram(id);
}

GLuint Shader::getId()
{
	return id;
}

void Shader::addOptionalUniform(const char* name)
{
	GLuint uForm = glGetUniformLocation(id, name);
	optional.push_back(uForm);
}

void Shader::use(glm::mat4 m, glm::mat4 v, glm::mat4 p)
{
	glUseProgram(id);
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m));
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(v));
	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
}

void Shader::use(glm::mat4 m, glm::mat4 v, glm::mat4 p, const std::vector<glm::vec3> optional_values)
{
	glUseProgram(id);
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m));
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(v));
	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
	for (int i = 0; i < optional.size(); i++) {
		glUniform3f(optional[i], optional_values[i].x, optional_values[i].y, optional_values[i].z);
	}
}

Shader::~Shader()
{
	glDeleteProgram(id);
}
