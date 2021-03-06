#version 410
attribute vec4 coord;
attribute vec3 norm;
attribute vec2 texcoord;
attribute vec3 color;

// параметры преобразований
uniform mat4 transform_model;
uniform mat4 transform_viewProjection;
uniform mat3 transform_normal;
uniform vec3 transform_viewPosition;

// параметры точечного источника освещения
uniform vec4 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec3 light_attenuation;

// параметры материала
uniform sampler2D material_texture;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform vec4 material_emission;
uniform float material_shininess;

out vec2 vert_texcoord;   // текстурные координаты
out vec4 vert_color;

void main(){
	vec4 vertex = transform_model * coord;
	vec4 lightDir4 = light_position - vertex;
	gl_Position = transform_viewProjection * vertex;

	vec3 normal = normalize(transform_normal * norm);
	vec3 lightDir = normalize(vec3(lightDir4));
	vec3 viewDir = normalize(transform_viewPosition - vec3(vertex));
	float vert_distance = length(lightDir4);
	
	float attenuation = 1;
	if(length(light_attenuation) != 0)
		 attenuation = 1.0 / (light_attenuation[0] + light_attenuation[1] * vert_distance + light_attenuation[2] * vert_distance * vert_distance);
	
	vec4 colorW = material_emission;
	colorW += material_ambient * light_ambient * attenuation;
	float Ndot = max(dot(normal, lightDir), 0.0);
	colorW += material_diffuse * light_diffuse * Ndot * attenuation;
	
	// добавление отражённого света
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material_shininess), 0.0);
	colorW += material_specular * light_specular * RdotVpow * attenuation;
	
	vert_texcoord = texcoord;
	//vert_color = vec4(1,0,0,1);
	vert_color = colorW;
}
