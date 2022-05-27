#version 400
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 bloom;
layout (location = 2) out uvec4 index;

in vec3 normal;
in vec3 fpos;
in vec2 textCoord;

uniform vec3 viewPos;
uniform vec3 sunPos;
uniform float sunRadius;
uniform mat4 modelM;
uniform sampler2D currentTexture;
uniform int id;

#define PI 3.141592
#define SAMPLES 50
#define LIGHT_CONSTANT 0.025f
#define LIGHT_LINEAR 20.0f

//probabil trebuie optimizat ce fac pe aici...
vec3 lighting(vec3 fpos, vec3 normal, vec3 viewPos,
				vec3 ambient, vec3 lightColor, vec3 specular, float specPower)
{
	vec3 pos = (modelM * vec4(fpos, 1.0f)).xyz;

	// fibonacci sphere / golden spiral / trebuie sa aflu cum functioneaza chestia asta
	float phi = PI * (3.0f - sqrt(5.0f));
	vec3 col = vec3(0.0f);
	vec3 lightPos;
	int i;
	for(i = 0; i < SAMPLES; ++i)
	{
		lightPos.y = 1 - (i * 1.0f / (SAMPLES - 1));
		
		float radius = sqrt(1 - lightPos.y * lightPos.y);
		float theta = phi * i;

		lightPos.x = cos(theta) * radius;
		lightPos.z = sin(theta) * radius;

		//iau in calcul raza si pozitia soarelui
		lightPos = lightPos * sunRadius;
		lightPos = lightPos + sunPos;

		vec3 L = normalize(lightPos - pos);
		vec3 V = normalize(viewPos - pos);
		vec3 N = normalize(normal);
		vec3 R = normalize(reflect(-L, N));

		float diffCoef = max(dot(L,N),0);
		float dotSpec = max(dot(R,V),0);
		float specCoef = pow(dotSpec, specPower);

		vec3 ambientColor = ambient * lightColor;
		vec3 diffuseColor = diffCoef * lightColor;
		vec3 specularColor = specCoef * specular * lightColor;

		float dist = length(lightPos - pos);
		float attenuation = (LIGHT_CONSTANT + pow(2, -1.0f * log(dist)) / LIGHT_LINEAR);
		ambientColor  *= attenuation;
		diffuseColor  *= attenuation;
		col = col + ( ambientColor + diffuseColor + specularColor ); 
	}

	//camera lumina
	vec3 L = normalize(viewPos - pos);
	vec3 V = normalize(viewPos - pos);
	vec3 N = normalize(normal);
	vec3 R = normalize(reflect(-L, N));

	float diffCoef = max(dot(L,N),0) * SAMPLES;
	float dotSpec = max(dot(R,V),0);
	float specCoef = pow(dotSpec, specPower);

	vec3 ambientColor = ambient * SAMPLES * lightColor;
	vec3 diffuseColor = diffCoef * lightColor;
	vec3 specularColor = specCoef * SAMPLES * specular * lightColor;

	float dist = length(viewPos - pos);
	float attenuation = 1.0 / (1.0f + 0.22f * dist + 0.20f * (dist * dist));  
	ambientColor  *= attenuation;
	diffuseColor  *= attenuation;
	specularColor *= attenuation;
	col = col + (diffuseColor + specularColor ); 

	return clamp(col, 0, 1);
}


void main() 
{
	vec4 textureColor = texture(currentTexture, textCoord);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 ambient = vec3(3.0f / SAMPLES);
	vec3 specular = vec3(7.0f / SAMPLES);
	float specPower = 64;
	
	vec3 color = lighting(fpos, normal, viewPos, ambient, lightColor, specular, specPower);
		
	fragColor = textureColor * vec4(color, 1.0);
	bloom = vec4(0.0f,0.0f,0.0f,1.0f);
	index = uvec4(0, gl_FragCoord.xy , id);
}
