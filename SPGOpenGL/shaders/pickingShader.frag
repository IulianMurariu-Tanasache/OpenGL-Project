#version 400

uniform int objectIndex;

out uint FragColor;

void main()
{
	FragColor = objectIndex;
}