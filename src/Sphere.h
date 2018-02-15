#pragma once
#include "Source.h" //Include GLM (vec3, mat3, etc)
#include "Object.h"
#include "Ray.h"

class Sphere : public Object {
private:
	vec3 center;
	float radius;
	vec3 surfaceColor;
	vec3 reflectivity;
	vec3 opacity;
	const float Inf = std::numeric_limits<float>::max();

	float magnitude(vec3 a);
	float solveQuadratic(float a, float b, float c);

public:
	Sphere(vec3 &c, float &r, vec3 &sc, vec3 &rf, vec3 &op);

	virtual float intersectionPoint(Ray &ray);
	vec3 getNormal(Ray &ray, float t);


	vec3 getSurfaceColor() override { return surfaceColor; }
	vec3 getReflectivity() override { return reflectivity; }
	vec3 getOpacity() override { return opacity; }
};
