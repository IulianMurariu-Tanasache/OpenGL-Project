#include <GL/glew.h>
#include <GL/freeglut.h>

class FrameBuffer {
	public:
		GLuint fbo;
		GLuint colorBuffer;

		FrameBuffer();
		~FrameBuffer();

		virtual void init(unsigned int w_width, unsigned int w_height);
};