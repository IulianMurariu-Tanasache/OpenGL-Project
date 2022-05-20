#version 400
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out uvec4 index;

in vec2 textCoord;

uniform sampler2D currentTexture;

void main() 
{
	vec4 textureColor = texture(currentTexture, textCoord);
		
	FragColor = textureColor;
	BrightColor = textureColor;
	index = uvec4(0, gl_FragCoord.xy , 1);
}