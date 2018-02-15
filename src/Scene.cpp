class Scene {
public:
  std::vector<std::unique_ptr<PointLight>> lights;
  std::vector<std::unique_ptr<Object>> objects;
  Camera cam;
  int width;
  int height;

  Sphere(int width, int height) {
    this.width = width;
    this.height = height;
  }

  Sphere(int width, int height, Camera cam,
    std::vector<std::unique_ptr<PointLight>> lights,
    std::vector<std::unique_ptr<Object>> objects) {
      this.width = width;
      this.height = height;
      this.cam = cam;
      this.lights = lights;
      this.objects = objects;
    }

  void createTestScene() {
    this.cam = new Camera(vec3(0,0,200), vec3 (0,1,0), vec3(0,0,-1));
    lights.push_back(std::unique_ptr<PointLight>(new PointLight(vec3(0, 0, 200),
      vec3(0.5, 0.5, 0.5))));

    float r = 64;
  	float c = r * 2.1;
  	// middle
  	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(0.0, 0.0, -r), r,
  		vec3(1.00, 0.00, 0.00), vec3(0.2,0.2,0.2), vec3(0.5,0.5,0.5))));
  	// top
  	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(0.0, c, -r), r,
  		vec3(1.00, 1.00, 1.00), vec3 (0.2,0.2,0.2), vec3(1.0,1.0,1.0))));
  	// right
  	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(c, 0.0, -r), r,
  		vec3(1.00, 1.00, 0.00), vec3(0.2,0.2,0.2), vec3(1.0,1.0,1.0))));
  	// bottom
  	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(0.0, -c, -r), r,
  		vec3(0.00, 1.00, 0.00), vec3(0.2,0.2,0.2), vec3(1.0,1.0,1.0))));
  	// left
  	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(-c, 0.0, -r), r,
  		vec3(0.50, 1.00, 0.50), vec3(0.2,0.2,0.2), vec3(1.0,1.0,1.0))));
  }

}
