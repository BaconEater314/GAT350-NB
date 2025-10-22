#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_textcoord;

out vec3 v_color;
out vec2 v_textcoord;

uniform float u_time;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
	float frequency = 3.0;
	float amplitude = 0.3;
	vec3 position = a_position;

	v_color = a_color;
	v_textcoord = a_textcoord;

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}