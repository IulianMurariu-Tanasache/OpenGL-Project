#version 400
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

/*in vec3 normal;
in vec3 fpos;*/
in vec2 textCoord;

uniform sampler2D currentTexture;

void main() 
{
	vec4 textureColor = texture(currentTexture, textCoord);
		
	//vec3 color  = vec3(0.6, 0.3, 0);
	FragColor = textureColor;
	BrightColor = textureColor;
}