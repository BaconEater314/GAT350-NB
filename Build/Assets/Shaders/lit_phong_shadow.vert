#version 460 core

#define MAX_LIGHTS	5
#define POINT		0
#define DIRECTIONAL 1
#define SPOT		2

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out VS_OUT{
	vec3 position;
	vec2 texcoord;
	vec3 normal;
	vec4 shadowcoord;
	mat3 tbn;
} vs_out;


uniform struct Material
{
	vec3 baseColor;
	vec3 emissiveColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
	uint parameters;
};

uniform int u_numLights = 1;
uniform Material u_material;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_shadow_vp;

uniform vec3 u_ambient_light;

void main(){
	vs_out.texcoord = (a_texcoord * u_material.tiling) + u_material.offset;

	mat4 model_view = u_view * u_model;
	vs_out.position = vec3(model_view * vec4(a_position, 1));
	vs_out.normal = normalize(mat3(model_view) * a_normal);
	vs_out.shadowcoord = u_shadow_vp * u_model * vec4(a_position, 1);

	//vec3 N = normalize(normal_matrix * a_normal);
	//vec3 T = normalize(normal_matrix * a_tangent);
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}