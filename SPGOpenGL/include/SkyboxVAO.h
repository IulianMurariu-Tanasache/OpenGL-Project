#pragma once
#include "VAOObject.h"

class SkyboxVAO : public VAOObject
{
	public:
		SkyboxVAO(GLuint vbo);
		~SkyboxVAO();
		std::size_t stride() override;
		std::size_t offsettOf(const char* attribute) override;
		void bind() override;
};
