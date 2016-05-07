/* shader to update a particle system based on a simple kinematics function */
#version 410 core

layout (location = 0) in vec4 vbo;
layout (location = 1) in vec2 vt;

uniform mat4 view, proj;

out vec2	texture_coordinates;
out vec4	color;

void main() {

	if (vt.x > 1.0f)
		color = vec4(1.0, 0.0, 0.0, 1.0);

	texture_coordinates = vt;

	gl_Position = proj * view * vbo;
}
