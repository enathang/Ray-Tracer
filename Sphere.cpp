class Sphere : public Object {
public:
	vec3 center;
	float radius;
	vec3 surfaceColor;
	vec3 reflectivity;
	vec3 opacity;

	Sphere(vec3 &c, float &r, vec3 &sc, vec3 &rf, vec3 &op) {
		center = c;
		radius = r;
		surfaceColor = sc;
		reflectivity = rf;
		opacity = op;
	}

	float intersectionPoint(Ray &ray) override {
		vec3 p1 = ray.direction; //-rayOrigin IMPORTANT FOR LATER
		vec3 k = (ray.origin - center);
		float a = glm::dot(p1, p1);
		float b = 2 * glm::dot(p1, k);
		float c = glm::dot(k, k) - (radius*radius);
		float collisionPoint = solveQuadratic(a, b, c);

		return collisionPoint;
	}

	vec3 getNormal(Ray &ray, float t) override {
		vec3 P = ray.origin + ray.direction*vec3(t,t,t);
		float mag = magnitude(P - center);
		vec3 normal = (P - center)/vec3(mag,mag,mag);

		return normal;
	}
	vec3 getSurfaceColor() override { return surfaceColor; }
	vec3 getReflectivity() override { return reflectivity; }
	vec3 getOpacity() override { return opacity; }
};
