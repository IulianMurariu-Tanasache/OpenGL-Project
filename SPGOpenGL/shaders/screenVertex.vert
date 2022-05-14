#version 400

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTextCoord;

out vec2 textCoord;

void main() 
{
	gl_Position = vec4(vPos, 1.0);
	textCoord = vTextCoord;
}