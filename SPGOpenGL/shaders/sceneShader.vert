#version 400

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTextCoord;

out vec2 textCoord;

uniform mat4 mvp;

void main() 
{
	gl_Position = mvp * vec4(vPos, 1.0);
	textCoord = vTextCoord;
}