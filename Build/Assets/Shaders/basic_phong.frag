#version 460 core

#define MAX_LIGHTS	5
#define POINT		0
#define DIRECTIONAL 1
#define SPOT		2

#define BASE_MAP		(1 << 0)
#define SPECULAR_MAP	(1 << 1)
#define EMISSIVE_MAP	(1 << 2)

in VS_OUT{
	vec3 position;
	vec2 texcoord;
	vec3 normal;
} fs_in;

uniform struct Light
{
	int type;
	vec3 position;
	vec3 color;
	vec3 direction;
	float intensity;
	float range;
	float outerSpotAngle;
	float innerSpotAngle;
};

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
uniform Light u_lights[MAX_LIGHTS];
uniform Material u_material;

uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;

uniform vec3 u_ambient_light;

out vec4 f_color;

float calculateAttenuation(in float light_distance, in float range)
{
	float attenuation = max(0.0, (1.0 - (light_distance / range)));
	return attenuation * attenuation;
}

vec3 calulateLight(in Light light, in vec3 position, in vec3 normal, in float specularMask){ 
	
	vec3 light_dir;
	float attenuation;
	float light_distance;

	switch (light.type)
	{
		case POINT:
			light_dir = normalize(light.position - position);
			light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);
		break;

		case DIRECTIONAL:
			light_dir = normalize(light.direction);
			attenuation = 1.0;
		break;

		case SPOT:
			light_dir = normalize(light.position - position);
			light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);

			float angle = acos(dot(light_dir, light.direction));
			if(angle > light.outerSpotAngle) attenuation = 0.0;
			else attenuation *= smoothstep(light.outerSpotAngle, light.innerSpotAngle, angle);;

			break;
	}

	//diffuse
	float NdotL = max(dot(light_dir, normal), 0); 
	vec3 diffuse = light.color * NdotL * u_material.baseColor;

	//sepcular
	vec3 view_dir = normalize(-position);

	// blinn phong
	vec3 halfway_dir = normalize(light_dir + view_dir);
	float NdotH = max(dot(normal, halfway_dir), 0);
	NdotH = pow(NdotH, u_material.shininess);
	vec3 specular = vec3(NdotH) * specularMask;

	// phong
	//vec3 reflection = reflect(-light_dir, normal);
	//float RdotV = max(dot(reflection, view_dir), 0);
	//RdotV = pow(RdotV, u_material.shininess);
	//vec3 specular = vec3(RdotV);

	return (diffuse + specular) * light.intensity * attenuation;
}

void main(){
	float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u)
	? texture(u_emissiveMap, fs_in.texcoord).r
	: 1;

	vec3 color = u_ambient_light;
	for(int i = 0; i < u_numLights; i++){
		color += calulateLight(u_lights[i], fs_in.position, fs_in.normal, specularMask);
	}

	vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u) 
	? texture(u_emissiveMap, fs_in.texcoord) * vec4(u_material.emissiveColor, 1)
	: vec4(u_material.emissiveColor, 1);

	//f_color = texture(u_baseMap, fs_in.texcoord) * vec4(color, 1) + emissive;
	f_color = vec4(gl_FragCoord.z);
}