#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

class MainFrameBuffer {
	private:
		GLuint fbo;
		GLuint depthBuffer;

	public:
		GLuint colorBuffers[3];
		MainFrameBuffer() {};
		~MainFrameBuffer();

		void init(unsigned int w, unsigned int h);
		void enable() const;
		void disable() const;
};