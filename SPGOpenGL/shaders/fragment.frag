#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 fpos;
in vec2 textCoord;

//uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 sunPos;
uniform float sunRadius;
uniform mat4 modelM;
uniform sampler2D currentTexture;

#define PI 3.141592
#define SAMPLES 50
#define LIGHT_CONSTANT 0.025f
#define LIGHT_LINEAR 20.0f

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
		specularColor *= attenuation;
		col = col + ( ambientColor + diffuseColor + specularColor ); 
	}

	return clamp(col, 0, 1);
}


void main() 
{
	vec4 textureColor = texture(currentTexture, textCoord);
	//vec3 objectColor =  textureColor.xyz; //vec3(1.0, 0.0, 1.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 ambient = vec3(0.5f / SAMPLES);
	vec3 specular = vec3(0.7f / SAMPLES);
	float specPower = 32;
	
	vec3 color = lighting(fpos, normal, viewPos, ambient, lightColor, specular, specPower);
		
	//vec3 color  = vec3(0.6, 0.3, 0);
	fragColor = textureColor * vec4(color, 1.0);
}
