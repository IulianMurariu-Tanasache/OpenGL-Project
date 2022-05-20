#version 400

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTextCoord;

uniform mat4 modelViewProjectionMatrix;

void main() 
{
	gl_Position = modelViewProjectionMatrix * vec4(vPos, 1.0);
}