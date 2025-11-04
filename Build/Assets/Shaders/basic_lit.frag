#version 460 core

in vec2 v_textcoord;
in vec3 v_color;

out vec4 f_color;

in VS_OUT
{
	vec2 texcoord;
	vec3 color;
} fs_in;


uniform sampler2D u_texture;

uniform struct Material
{
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
} u_material;

void main(){
	f_color = texture(u_material.baseMap, fs_in.texcoord) * vec4(fs_in.color, 1);
}