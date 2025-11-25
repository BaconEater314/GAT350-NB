#version 460 core

#define MAX_LIGHTS	5
#define POINT		0
#define DIRECTIONAL 1
#define SPOT		2

#define BASE_MAP		(1 << 0)
#define SPECULAR_MAP	(1 << 1)
#define EMISSIVE_MAP	(1 << 2)
#define NORMAL_MAP		(1 << 3)
#define CUBE_MAP		(1 << 4)

in VS_OUT{
	vec3 position;
	vec2 texcoord;
	vec3 normal;
	mat3 tbn;
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
uniform sampler2D u_normalMap;

in vec2 v_texcoord;
out vec4 f_color;

uniform vec3 u_ambient_light;


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
			light_dir = -light.direction;
		break;

		case SPOT:
			light_dir = normalize(light.position - position);
			light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);

			float angle = acos(dot(light_dir, -light.direction));
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
	vec4 color = texture(u_baseMap, v_texcoord);
	vec4 postprocess = color;

	float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u)
	? texture(u_emissiveMap, fs_in.texcoord).r
	: 1;

	//color = u_ambient_light;
	for(int i = 0; i < u_numLights; i++){
		color += calulateLight(u_lights[i], fs_in.position, fs_in.normal, specularMask);
	}

	vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u) 
	? texture(u_emissiveMap, fs_in.texcoord) * vec4(u_material.emissiveColor, 1)
	: vec4(u_material.emissiveColor, 1);

	if ((u_parameters & GRAYSCALE) != 0u) postprocess = vec4(vec3((postprocess.r + postprocess.g + postprocess.b) / 3.0), postprocess.a);
	if ((u_parameters & COLORTINT) != 0u) postprocess = postprocess * vec4(u_colorTint, 1);
	if ((u_parameters & SCANLINE) != 0u) postprocess = (int(gl_FragCoord.y) % 3 != 0) ? vec4(0, 0, 0, 1) : postprocess;
	if ((u_parameters & GRAIN) != 0u)  postprocess = postprocess * random(gl_FragCoord.xy + u_time);
	if ((u_parameters & INVERT) != 0u)  postprocess = vec4(1) - postprocess;
	
	//f_color = mix(color, postprocess, u_blend);
}