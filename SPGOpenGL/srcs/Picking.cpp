#include "PickingBuffer.h"
#include <iostream>

void PickingBuffer::init(unsigned int w_width, unsigned int w_height)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, w_width, w_height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w_width, w_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
	// also check if framebuffers are complete (no need for depth buffer)
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PickingBuffer::enableWriting() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void PickingBuffer::disableWritting() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void PickingBuffer::PixelInfo::print() 
{
	std::cout << "S-a apasat pe" << ObjectID << '\n';
}

PickingBuffer::~PickingBuffer()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &colorBuffer);
	glDeleteTextures(1, &depthBuffer);
}

PickingBuffer::PixelInfo PickingBuffer::readPixel(unsigned int x, unsigned int y)
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	PixelInfo pixel;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel);
	//std::cout << glGetError() << '\n';
	glReadBuffer(GL_NONE);
	glBindBuffer(GL_READ_FRAMEBUFFER, 0);

	return pixel;
}