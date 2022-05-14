#include <GL/glew.h>
#include <GL/freeglut.h>

class FrameBuffer {
	public:
		GLuint fbo;
		GLuint colorBuffer;

		FrameBuffer();
};