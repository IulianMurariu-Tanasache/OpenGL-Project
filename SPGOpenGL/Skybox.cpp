#include "Skybox.h"

Skybox::Skybox(std::vector<std::string>& faces, int flip)
{
	texture = new SkyboxTexture(faces, flip);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	vaoObj = new SkyboxVAO(vbo);

	dataSize = sizeof(skyboxVertices);
}

Skybox::~Skybox()
{
	delete texture;
	texture = nullptr;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
}

void Skybox::bind() 
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	vaoObj->bind();
	texture->bind();
}
