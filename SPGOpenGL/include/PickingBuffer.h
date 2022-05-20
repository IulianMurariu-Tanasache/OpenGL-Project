#include "FrameBuffer.h"

class PickingBuffer : public FrameBuffer {
	public:
		struct PixelInfo {
			unsigned int ObjectID = 0;

			void print();

		};

		GLuint depthBuffer;

		PickingBuffer() {};
		~PickingBuffer();
		void init(unsigned int w_width, unsigned int w_height) override;
		void enableWriting() const;
		void disableWritting() const;
		PixelInfo readPixel(unsigned int x, unsigned int y);


};