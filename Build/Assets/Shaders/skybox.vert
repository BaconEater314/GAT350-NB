#version 460 core

layout (location = 0) in vec3 a_position;

out vec3 v_textcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
	float frequency = 3.0;
	float amplitude = 0.3;
	vec3 position = a_position;

	v_textcoord = a_position;

	// remove translation of camera
	mat4 vp = u_projection * mat4(mat3(u_view));
	gl_Position = vp * vec4(a_position, 1.0);
}