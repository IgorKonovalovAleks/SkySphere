#include "SphereElements.h"

SphereElements::SphereElements() {
	flat_shader = Shader(vertex, geo, fragment, "model", "view", "proj");
	flat_shader.addOptionalUniform("light_position");
	flat_shader.addOptionalUniform("light_color");
	sphere_data = VAO();
	sphere_data.addVertexBufferObject(vertex_data, { 1, 3 }, 4, { 0, 1 });
	
	glm::mat4 rot;
	flat_rotate_data.push_back(glm::rotate(rot, 10.0f, glm::vec3(0.0, 1.0, 0.0)) * glm::rotate(rot, 23.15f, glm::vec3(0.0, 0.0, 1.0)));	
	flat_rotate_data.push_back(rot);
	flat_rotate_data.push_back(rot);
	flat_rotate_data.push_back(rot);

}

void SphereElements::draw(glm::mat4 view, glm::mat4 projection, const glm::vec3 light_position, const glm::vec3 light_color)
{
	std::cout << "start draw" << std::endl;
	for (int i = 0; i < 4; i++) {
		flat_shader.use(flat_rotate_data[i], view, projection, { light_position, light_color });
		sphere_data.drawVxStrip(1, i);
	}
	std::cout << "end draw" << std::endl;
}
