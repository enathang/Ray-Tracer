class Camera {
public:
	vec3 origin;
	vec3 up;
	vec3 direction;

	Camera(vec3 &o, vec3 &u, vec3 &d) {
		origin = o;
		up = u;
		direction = d;
	}
};
