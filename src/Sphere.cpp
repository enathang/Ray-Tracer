#include "Source.h" //Include GLM (vec3, mat3, etc)
#include "Object.h"
#include "Ray.h"
#include "Sphere.h"
	float Sphere::solveQuadratic(float a, float b, float c) {
		float discriminant = (b*b) - (4 * a*c);
		if (discriminant < 0) return Inf;
		if (discriminant == 0) return -b / (2 * a);
		float result1 = (-b + sqrt(discriminant)) / (2 * a);
		float result2 = (-b - sqrt(discriminant)) / (2 * a);
		if (result1 < 0) result1 = Inf; //Clean this up
		if (result2 < 0) result2 = Inf;
		if (result1 > result2) return result2;
		else return result1;
	}

	float Sphere::magnitude(vec3 a) {
		return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	}

	Sphere::Sphere(vec3 &c, float &r, vec3 &sc, vec3 &rf, vec3 &op) {
		center = c;
		radius = r;
		surfaceColor = sc;
		reflectivity = rf;
		opacity = op;
	}

	float Sphere::intersectionPoint(Ray &ray) {
		vec3 p1 = ray.direction; //-rayOrigin IMPORTANT FOR LATER
		vec3 k = (ray.origin - center);
		float a = glm::dot(p1, p1);
		float b = 2 * glm::dot(p1, k);
		float c = glm::dot(k, k) - (radius*radius);
		float collisionPoint = solveQuadratic(a, b, c);

		return collisionPoint;
	}

	vec3 Sphere::getNormal(Ray &ray, float t) {
		vec3 P = ray.origin + ray.direction*vec3(t, t, t);
		float mag = magnitude(P - center);
		vec3 normal = (P - center) / vec3(mag, mag, mag);

		return normal;
	}
