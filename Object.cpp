//Types of instances in scene
class Object {
public:
	vec3 surfaceColor = vec3(0.5,0,0.5);
	virtual float intersectionPoint(Ray &) = 0;
	virtual vec3 getNormal(Ray &, float) = 0;
	virtual vec3 getSurfaceColor() = 0;
	virtual vec3 getReflectivity() = 0;
	virtual vec3 getOpacity() = 0;
};
