#version 460 core

#define MAX_LIGHTS 5

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textcoord;
layout (location = 2) in vec3 a_normal;

out VS_OUT
{
	vec2 texcoord;
	vec3 color;
} vs_out;

out vec2 v_textcoord;
out vec3 v_color;


uniform struct Light
{
	vec3 position;
	vec3 color;

	float intensity;
	float range;
};

uniform struct Material
{
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
};

uniform int u_numLights = 1;
uniform Light u_lights[MAX_LIGHTS];
uniform Material u_material;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambient_light;

float calculateAttenuation(in float light_distance, in float range)
{
	float attenuation = max(0.0, (1.0 - (light_distance / range)));
	return attenuation * attenuation;
}

vec3 calulateLight(in Light light, in vec3 position, in vec3 normal){
	//diffuse
	vec3 light_dir = normalize(light.position - position);
	float intensity = max(dot(light_dir, normal), 0); 
	vec3 diffuse = light.color * intensity * u_material.baseColor;

	//sepcular
	vec3 reflection = reflect(-light_dir, normal);
	vec3 view_dir = normalize(-position);
	intensity = max(dot(reflection, view_dir), 0);
	intensity = pow(intensity, u_material.shininess);
	vec3 specular = vec3(intensity);

	float light_distance = length(light.position - position);
	float attenuation = calculateAttenuation(light_distance, light.range);

	return (diffuse + specular) * light.intensity * attenuation;
}

void main(){
	vs_out.texcoord = (a_textcoord * u_material.tiling) + u_material.offset;

	mat4 model_view = u_view * u_model;
	vec3 position = vec3(model_view * vec4(a_position, 1));
	vec3 normal = normalize(mat3(model_view) * a_normal);

	vs_out.color = u_ambient_light;
	for (int i = 0; i < u_numLights; i++){
		vs_out.color += calulateLight(u_lights[i], position, normal);
	}

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}