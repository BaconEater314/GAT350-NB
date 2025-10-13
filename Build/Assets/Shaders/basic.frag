#version 460 core

in vec3 v_color;
in vec2 v_textcoord;

out vec4 f_color;

uniform float u_time;
uniform sampler2D u_texture;

void main(){
	//f_color = vec4(1,0,0,1);
	//float offset = sin(u_time + gl_FragCoord.x);
	//f_color = vec4(v_color / offset, 1);

	f_color = texture(u_texture, v_textcoord);
}