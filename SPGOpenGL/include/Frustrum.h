#pragma once
#include "Plane.h"
#include "Camera.h"

class Frustrum {
	public:
		Plane topPlane;
		Plane bottomPlane;
		Plane leftPlane;
		Plane rightPlane;
		Plane nearPlane;
		Plane farPlane;
		
		Frustrum(const Camera& cam);
};