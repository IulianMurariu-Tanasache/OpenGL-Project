#include "Skybox.h"

Skybox::Skybox(std::vector<std::string>& faces, int flip)
{
	texture = new SkyboxTexture(faces, flip);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	vaoObj = new SkyboxVAO(vbo);
}

Skybox::~Skybox()
{
	delete texture;
	delete vaoObj;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
}

void Skybox::draw() 
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	vaoObj->bind();
	texture->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
