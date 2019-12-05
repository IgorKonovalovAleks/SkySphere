#include "SphereElements.h"

SphereElements::SphereElements() {
	flat_shader = Shader(vertex, geo, fragment, "model", "view", "proj");
	flat_shader.addOptionalUniform("light_position");
	flat_shader.addOptionalUniform("light_color");
	sphere_data = VAO();
	sphere_data.addVertexBufferObject(data, { 3, 3 }, 6, { 0, 3 });
}

void SphereElements::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, const glm::vec3& light_position, const glm::vec3& light_color)
{
	flat_shader.use(model, view, projection, std::vector<glm::vec3> { light_position, light_color });
	sphere_data.drawVxStrip(1, 1);
	/*
	flat_shader.use(model, view, projection, { light_position, light_color });
	sphere_data.drawVxStrip(1, 1);
	flat_shader.use(model, view, projection, { light_position, light_color });
	sphere_data.drawVxStrip(1, 2);
	flat_shader.use(model, view, projection, { light_position, light_color });
	sphere_data.drawVxStrip(1, 3);
	*/
}
