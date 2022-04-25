#include "Object.h"

Object::Object(FlyweightObjectComponent* component) 
{
	//reset();
	visible = true;
	baseData = component;
}