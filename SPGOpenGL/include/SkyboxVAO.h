#pragma once
#include "VAOObject.h"

class SkyboxVAO : public VAOObject
{
	public:
		//std::map<const char*, std::size_t> attributeMap;
		//GLuint vaoObj;

		SkyboxVAO(GLuint vbo);
		~SkyboxVAO();
		std::size_t stride() override;
		std::size_t offsettOf(const char* attribute) override;
		void bind() override;
};
