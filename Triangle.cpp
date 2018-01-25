class Triangle : public Object {
public:
	vec3 p0;
	vec3 p1;
	vec3 p2;
	vec3 surfaceColor;
	Triangle(vec3 &v1, vec3 &v2, vec3 &v3) {
		p0 = v1;
		p1 = v2;
		p2 = v3;
	}
	vec3 normal = glm::normalize(glm::cross(p1-p0, p2-p0));
	float intersectionPoint(Ray &ray) {

	}
	vec3 getNormal(Ray &, float) override { return normal; }
	vec3 getSurfaceColor() { return surfaceColor; }

};
