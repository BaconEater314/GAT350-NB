#version 460 core

in vec3 v_color;
in vec2 v_textcoord;

out vec4 f_color;

uniform float u_time;
uniform sampler2D u_texture;

void main(){
	f_color = texture(u_texture, v_textcoord);
}