#version 460 core

in vec3 v_textcoord;

out vec4 f_color;

uniform samplerCube u_cubeMap;

void main(){
	f_color = texture(u_cubeMap, v_textcoord);
}