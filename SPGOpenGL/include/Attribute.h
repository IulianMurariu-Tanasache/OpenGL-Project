#pragma once
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

struct Attribute {
	Attribute(std::size_t _stride, std::size_t _offsett)
	{
		//number = _nr;
		stride = _stride;
		offsett = (void*)_offsett;
	}
	Attribute() {}
	std::vector< glm::vec3 > data;
	//int number;?
	//GLenum type?
	std::size_t stride; //distance between consecutive elements of same attribute
	void* offsett; //where is the first element of this attibute
		
};