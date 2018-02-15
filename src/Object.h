#pragma once
#include "Source.h" //Include GLM (vec3, mat3, etc)
#include "Ray.h"
class Object {
public:
	virtual float intersectionPoint(Ray &) = 0;
	virtual vec3 getNormal(Ray &, float) = 0;
	virtual vec3 getSurfaceColor() = 0;
	virtual vec3 getReflectivity() = 0;
	virtual vec3 getOpacity() = 0;
};
