#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <memory>

#include "Triangle.h"
#include "Object.h"
#include "Ray.h"
#include "Sphere.h"

#include "Source.h" //Include GLM (vec3, mat3, etc)
#include "bitmap_image.hpp" //Include .bmp functions (new image, .setpixel, etc)

struct PointLight {
	vec3 position;
	vec3 lumocity;
};

struct Camera {
	vec3 origin;
	vec3 up;
	vec3 direction;
};

void log(char *message) {
	std::cout << message << std::endl;
}

void log(char *name, vec3 vec) {
	std::cout << name << ": " << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;
}

void log(char *name, float i) {
	std::cout << name << ": " << i << std::endl;
}

//Math stuff
const float Inf = std::numeric_limits<float>::max();

float magnitude(vec3 a) {
	return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

float calculateTheta(vec3 a, vec3 b) {
	//Formula: |a|*|b|*cos(theta) = a dot b -> theta = cos-1(a dot b/|a|*|b|)
	float a_ = magnitude(a);
	float b_ = magnitude(b);
	float theta = acosf(glm::dot(a, b) / (a_*b_));
	return theta;
}

/*=====Find the closest object on a ray=====*/
int closestHitObject(Ray &ray, std::vector<std::unique_ptr<Object>> &objects, Object *&hitObject) {

	/*=====Loop through each object to find closest=====*/
	float tNearest = Inf;
	std::vector<std::unique_ptr<Object>>::const_iterator iter = objects.begin();
	for (; iter != objects.end(); iter++) {
		float t = (*iter)->intersectionPoint(ray);
		vec3 col = (*iter)->getSurfaceColor();
		if (t < tNearest) {
			tNearest = t;
			hitObject = iter->get();
		}
	}

	return tNearest;
}

/*=====Find sum of lights on point=====*/
vec3 castLightRay(Ray &ray, Ray &normal, Object *&hitObject, std::vector<std::unique_ptr<PointLight>> &lights, std::vector<std::unique_ptr<Object>> &objects, int level) {

	if (level > 10) { return vec3(0, 0, 0); } //If max level return dark
	/*=====Create light color=====*/
	vec3 light = vec3(0, 0, 0);

    /*=====Loop through lights=====*/
	std::vector<std::unique_ptr<PointLight>>::const_iterator iter = lights.begin();
	for (; iter != lights.end(); iter++) {

		/*=====Find ray to light=====*/
		vec3 pos = (*iter)->position;
		vec3 lightRayDir = glm::normalize(pos - normal.origin);
		Ray lightRay = { normal.origin, lightRayDir };
		float tLight = vec3((pos - normal.origin)/lightRayDir)[0];

        /*=====Find closest object on same ray (if any)=====*/
		float tNearestObject = closestHitObject(lightRay, objects, hitObject);
		tNearestObject = Inf;
		std::vector<std::unique_ptr<Object>>::const_iterator iter1 = objects.begin();
		for (; iter1 != objects.end(); iter1++) {
			float t = (*iter1)->intersectionPoint(lightRay);
			if (t < tNearestObject) {
				tNearestObject = t;
			}
		}

		/*=====Check the object isn't blocking the light=====*/
		if (tLight < tNearestObject) {

			/*=====Calculate lighting on object=====*/
			float theta = glm::dot(normal.direction, lightRayDir);
			theta = glm::max(0.0f, theta);
			if (theta > 1) theta = 1;
			light += (*iter)->lumocity * vec3(theta, theta, theta);
		}
		else {
			//Do nothing, because an object is in the way
		}
	}

	/*=====Return the light=====*/
	return light;
}

/*=====Find closest visible object (if any) and calculate color+lighting =====*/
vec3 castRay(Ray &cameraRay, std::vector<std::unique_ptr<PointLight>> &lights, std::vector<std::unique_ptr<Object>> &objects, int level) {

	if (level > 5) { return vec3(0, 0, 0); } //If maxed out, return background color
	/*=====Create the pixel color and set to black=====*/
	vec3 color = vec3(0, 0, 0);

	/*=====Find closest visible object=====*/
	Object *hitObject = nullptr;
	float t = closestHitObject(cameraRay, objects, hitObject);

	//if (hitObject != nullptr) { return vec3(1, 1, 1); } else { return vec3(0, 0, 0);} //Black and white collision test

	/*=====If there's a visible object, calculate color of that object=====*/
	if (hitObject != nullptr) {
		/*=====Calculate normal=====*/
		vec3 normalOrigin = cameraRay.origin + cameraRay.direction*vec3(t, t, t);
		vec3 normalDirection = hitObject->getNormal(cameraRay, t);
		normalOrigin += normalDirection*vec3(0.001); //start light ray a little bit removed
		Ray normal = { normalOrigin, normalDirection };

		/*=====Calculate sum of lighting/reflections=====*/
		vec3 surfaceColor = hitObject->getSurfaceColor();
		vec3 li = vec3(0, 0, 0);                                                        //self-illumination
		vec3 ol = castLightRay(cameraRay, normal, hitObject, lights, objects, 0);       //outside-illumination
		vec3 gi = surfaceColor*(0.2, 0.2, 0.2);											//global-illumination
		/*=====Calculate reflection=====*/
		//Equation copied from www.cs.unc.edu
		float c1 = -glm::dot(normal.direction, cameraRay.direction);
		vec3 R1 = cameraRay.direction + (vec3(2) * normal.direction*vec3(c1));
		Ray reflectionRay = { normalOrigin, R1 };
		vec3 reflection = castRay(reflectionRay, lights, objects, level+1)*hitObject->getReflectivity();

		/*=====Calculate reflection=====*/
		//Equation copied from www.cs.unc.edu
		vec3 opacity = hitObject->getOpacity();
		float ref = 1;
		float c2 = sqrt(1 - ref*ref*(1-c1*c1));
		vec3 r2 = vec3(ref)*cameraRay.direction + vec3(ref*c1 - c2)*normal.direction;
		Ray refractionRay = { normalOrigin - normalDirection * vec3(0.002), r2 };//Put origin slightly inside sphere
		vec3 refraction = castRay(refractionRay, lights, objects, level+1)*opacity;

		color = li + (ol + gi + reflection)*opacity + refraction*(vec3(1)-opacity);
	}

	/*=====Otherwise calculate the background color=====*/
	else {
		color = vec3(0, 0, 0);
	}

	/*=====Return the color of the pixel=====*/
	return color;
}

/*=====Renders the image by looping though each pixel and calculating the color=====*/
void render(Camera &camera, std::vector<std::unique_ptr<PointLight>> &lights, std::vector<std::unique_ptr<Object>> &objects, int width, int height) {

	/*=====Create frame buffer (made up of scan lines)=====*/
	/*For some reason, C++ doesn't like an expicit 2D vector for the frame buffer,
	  so we instead make an implicit 2D vector by making frame buffer a vector
	  of vectors (called scan lines)*/
	std::vector<std::vector<vec3>> frameBuffer;
	std::vector<vec3> scanLine;

	/*=====Variables to check render progress=====*/
	double iterationsSoFar = 0;
	double twentyFivePercent = width*height / 4;
	double fiftyPercent = twentyFivePercent * 2;
	double seventyFivePercent = twentyFivePercent * 3;

	/*=====Loop through each pixel and calculate color=====*/
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			/*=====For a pixel, find camera ray that passes through it=====*/
			vec3 rayOrigin = camera.origin;
			vec3 rayDirection = vec3((-width / 2 + i + 0.5)/1, (-height / 2 + j + 0.5)/1, camera.direction[2]);
			// log("rayDirection", rayDirection);
			Ray cameraRay = { rayOrigin, rayDirection };

			/*=====For a pixel, calculate the color of it=====*/
			vec3 color = castRay(cameraRay, lights, objects, 0);

			/*=====Push pixel color to scan line=====*/
			int red = glm::min((int)(color[0] * 255), 255);
			int green = glm::min((int)(color[1] * 255), 255);
			int blue = glm::min((int)(color[2] * 255), 255);
			scanLine.push_back(vec3(red, green, blue));

			/*=====Print out render progress=====*/
			iterationsSoFar++;
			if (iterationsSoFar == twentyFivePercent) { log("25% done"); }
			if (iterationsSoFar == fiftyPercent) { log("50% done"); }
			if (iterationsSoFar == seventyFivePercent) { log("75% done"); }
		}

		/*=====Push scan line to frame buffer=====*/
		frameBuffer.push_back(scanLine);
		scanLine.clear();
	}

	log("Render complete");

	/*=====Push frame buffer onto image=====*/
	bitmap_image image(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			vec3 pixelColor = frameBuffer[i][j];
			int red = pixelColor[0];
			int green = pixelColor[1];
			int blue = pixelColor[2];
			image.set_pixel(i, j, red, green, blue);
		}
	}

	/*=====Export image=====*/
	image.save_image("output2.bpm");
	log("Image saved");
	return;
}

int main(int argc, char **argv) {

	/*=====Options=====*/
	int width = 640;
	int height = 480;
	log("Establishing scene");

	/*=====Create camera=====*/
	Camera cam = { vec3(0,0,5), vec3(0,1,0), vec3(0,0,-1) };

	/*=====Create lights=====*/
	std::vector<std::unique_ptr<PointLight>> lights;
	lights.push_back(std::unique_ptr<PointLight>(new PointLight{ vec3(0, 0, 200), vec3(0.5, 0.5, 0.5) }));

	/*=====Create objects=====*/
	std::vector<std::unique_ptr<Object>> objects;
	float r = 64;//delete later

	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(0.0, 0.0, -r), r, vec3(1.00, 0.00, 0.00), vec3(0.2,0.2,0.2), vec3(0.5,0.5,0.5))));    //middle
	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(0.0, 2.1*r, -r), r, vec3(1.00, 1.00, 1.00), vec3 (0.2,0.2,0.2), vec3(1.0,1.0,1.0))));  //top
	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(2.1*r, 0.0, -r), r, vec3(1.00, 1.00, 0.00), vec3(0.2,0.2,0.2), vec3(1.0,1.0,1.0))));  //right
	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(0.0, -2.1*r, -r), r, vec3(0.00, 1.00, 0.00), vec3(0.2,0.2,0.2), vec3(1.0,1.0,1.0)))); //bottom
	objects.push_back(std::unique_ptr<Sphere>(new Sphere(vec3(-2.1*r, 0.0, -r), r, vec3(0.50, 1.00, 0.50), vec3(0.2,0.2,0.2), vec3(1.0,1.0,1.0)))); //left

    /*=====Render scene=====*/
	log("Scene established, beginning render");
	render(cam, lights, objects, width, height);

	return 0;
}
