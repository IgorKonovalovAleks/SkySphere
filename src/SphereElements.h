#pragma once
#include "Shader.h"
#include "VAO.h"

class SphereElements
{
private:

	VAO sphere_data;

	Shader flat_shader;

	const std::vector<GLfloat> data = 
	//d, horizontal, vertical, color
	{0.0, 0.0, 0.0, 1.0, 0.0, 0.0,                    //flats
	 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 
	 0.5, 0.0, (GLfloat) glm::radians(180.0f), 1.0, 0.0, 0.0, 
	 0.0, 0.0, (GLfloat) glm::radians(23.156f), 0.0, 1.0, 0.0,
	 
	 };

	const char* vertex =
		"#version 330 core\n"
		"layout (location = 0) in vec3 sph_coords;"
		"layout (location = 1) in vec3 col;"

		"out VS_OUT{"
		"  vec3 sphere_coords;"
		"  mat4 model;"
		"  mat4 view;"
		"  mat4 proj;"
		"  vec3 color;"
		"  vec4 frag_pos;"
		"} vs_out;"

		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		
		"void main() {"
		"  vs_out.sphere_coords = sph_coords;"
		"  vs_out.color = col;"
		"  gl_Position = proj * view * model * vec4(0.0, 0.0, 0.0, 1.0);"
		"  vs_out.frag_pos = model * vec4(0.0, 0.0, 0.0, 1.0);"
		"  vs_out.model = model;"
		"  vs_out.view = view;"
		"  vs_out.proj = proj;"
		"}";

	const char* geo =
		"#version 330 core\n"
		"layout (points) in;"
		"layout (line_strip, max_vertices = 40) out;"

		"in VS_OUT {"
		"  vec3 sphere_coords;"
		"  mat4 model;"
		"  mat4 view;"
		"  mat4 proj;"
		"  vec3 color;"
		"  vec4 frag_pos;"
		"} gs_in[];"

		"out vec3 color;"
		"out vec4 fragment_position;"

		"void main() {"
		"  color = gs_in[0].color;"
		"  float delta_h = radians(9.0);"
		"  float delta_v = radians(gs_in[0].sphere_coords.z / 10.0);"
		"  float d = gs_in[0].sphere_coords.x;"
		"  float r = sqrt(1.0 - pow(d, 2));"
		"  float h_angle = gs_in[0].sphere_coords.y;"
		"  float v_angle = 0.0;"
		"  for(int i = 0; i < 10; i++) {"
		"    fragment_position = vec4((r + d) * cos(v_angle) * sin(h_angle), (r + d) * sin(v_angle), (r + d) * cos(v_angle) * cos(h_angle), 1.0);"
		"    gl_Position = gs_in[0].proj * gs_in[0].view * gs_in[0].model * vec4((r + d) * cos(v_angle) * sin(h_angle), (r + d) * sin(v_angle), (r + d) * cos(v_angle) * cos(h_angle), 1.0);"
		"    EmitVertex();"
		"    h_angle += delta_h;"
		"    v_angle += delta_v;"
		"  }"
		"  for(int i = 0; i < 20; i++) {"
		"    fragment_position = vec4((r + d) * cos(v_angle) * sin(h_angle), (r + d) * sin(v_angle), (r + d) * cos(v_angle) * cos(h_angle), 1.0);"
		"    gl_Position = gs_in[0].proj * gs_in[0].view * gs_in[0].model * vec4((r + d) * cos(v_angle) * sin(h_angle), (r + d) * sin(v_angle), (r + d) * cos(v_angle) * cos(h_angle), 1.0);"
		"    EmitVertex();"
		"    h_angle += delta_h;"
		"    v_angle -= delta_v;"
		"  }"
		"  for(int i = 0; i < 10; i++) {"
		"    fragment_position = vec4((r + d) * cos(v_angle) * sin(h_angle), (r + d) * sin(v_angle), (r + d) * cos(v_angle) * cos(h_angle), 1.0);"
		"    gl_Position = gs_in[0].proj * gs_in[0].view * gs_in[0].model * vec4((r + d) * cos(v_angle) * sin(h_angle), (r + d) * sin(v_angle), (r + d) * cos(v_angle) * cos(h_angle), 1.0);"
		"    EmitVertex();"
		"    h_angle += delta_h;"
		"    v_angle += delta_v;"
		"  }"
		"  EndPrimitive();"
		"}";

	const char* fragment =
		"#version 330 core\n"
		"in vec4 fragment_position;"
		"in vec3 color;"
		
		"out vec4 frag_color;"
		"uniform vec3 light_color;"
		"uniform vec3 light_position;"

		"void main() {"
		"  float light_radius = 5.0;"
		"  float far_comp = sqrt(max((light_radius - length(fragment_position.xyz - light_position)) / light_radius, 0.0));"
		"  float ambient_strengh = 0.1;"
		"  vec3 ambient_color = ambient_strengh * light_color;"
		"  vec3 light_direction = normalize(light_position - fragment_position.xyz);"
		"  vec3 diffuse = max(dot(normalize(fragment_position.xyz), light_direction), 0.0) * far_comp * light_color;"
		"  vec3 res = (ambient_color + diffuse) * vec3(1.0, 1.0, 1.0);"
		"  if (res.x > 1.0) {"
		"    res.x = 1.0;"
		"  }"
		"  if (res.y > 1.0) {"
		"    res.y = 1.0;"
		"  }"
		"  if (res.z > 1.0) {"
		"    res.z = 1.0;"
		"  }"
		"  frag_color = vec4(color, 1.0);"
		"}";

public:
	SphereElements();
	void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, const glm::vec3& light_position, const glm::vec3& light_color);

};

