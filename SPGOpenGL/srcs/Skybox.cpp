#include "Skybox.h"

Skybox::Skybox(std::vector< char* >& paths, int flip = 0)
{
	texture = new SkyboxTexture(paths, flip);
	vaoObj = SkyboxVAO();
	dataSize = sizeof(skyboxVertices) / sizeof(float);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, dataSize, (void*)this->skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Skybox::~Skybox()
{
	delete texture;
	texture = nullptr;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
}