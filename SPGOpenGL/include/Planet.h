#pragma once
#include "Object.h"

class Planet : public Object {
	private:
		float axisRotAngle; // unghiul de rotatie in jurul propriei axe
		float axisRotAngleInc; // valoarea cu care se incrementeaza unghiul (de ex la apasarea unei taste)
		float orbitDist; // distanta fata de centrul orbitei
		float orbitAngle; // unghiul de rotatie pe orbita
		float orbitAngleInc; // valoarea cu care se incrementeaza unghiul (de ex la apasarea unei taste)
	public:
		Planet(
			std::shared_ptr<FlyweightObjectComponent> component, std::shared_ptr<Texture> texture, std::shared_ptr<VAOObject> vaoObj,
			float _axisRotAngle, float _axisRotAngleInc,
			float _orbitDist, float _orbitAngle, float _orbitAngleInc) :
			axisRotAngle(_axisRotAngle),
			axisRotAngleInc(_axisRotAngleInc), orbitDist(_orbitDist),
			orbitAngle(_orbitAngle), orbitAngleInc(_orbitAngleInc),
			Object(component, texture, vaoObj){}
		void move() override;
		glm::highp_mat4 rotateAroundAxis();
		glm::highp_mat4 rotateAroundOrbit();
		glm::highp_mat4 moveOnOrbit();
};
