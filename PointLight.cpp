class PointLight {
public:
	vec3 position;
	vec3 lumocity;
	PointLight(vec3 &p, vec3 &l) {
		position = p;
		lumocity = l;
	}
};
