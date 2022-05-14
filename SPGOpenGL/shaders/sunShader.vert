#version 400

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTextCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

out vec3 normal;
out vec3 fpos;
out vec2 textCoord;

void main() 
{
	gl_Position = modelViewProjectionMatrix * vec4(vPos, 1.0);
	
	normal = vNormal;
	normal = vec3(normalMatrix * vec4(normal, 1));

	fpos = vPos;
	textCoord = vTextCoord;
}