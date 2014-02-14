#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "Rays/Ray.h"
#include "Primitives/Sphere.h"
#include <cmath>
#include "../simplePPM.h"
#include "../objLoader.h"
#include <vector>
#include "../Scene.h"
#include "../buffer.h"
#include "Rays\RayGenerator.h"
#include <cmath>
#include <algorithm>

using namespace std;

class RayTracer {
public:

	RayTracer() {
		this->colorByNormal = false;
	}

	RayTracer(bool normColor) {
		this->colorByNormal = false;
	}

	~RayTracer() {
	}

	void drawFrameBuffer(char *fileName) {
		simplePPM_write_ppm(fileName, this->frameBuffer.getW(),
				this->frameBuffer.getH(),
				(unsigned char *) &this->frameBuffer.at(0, 0));
	}

	void setFrameBuffer(unsigned int w, unsigned int h) {
		this->frameBuffer = Buffer(w, h);
	}

	void renderRayDirectionsToImage(char *fileName) {
		RayGenerator rg = RayGenerator(this->scene.camera,
				this->frameBuffer.getW(), this->frameBuffer.getH());
		drawRaysToBuffer(this->frameBuffer, rg);
		drawFrameBuffer(fileName);
	}

private:
	Buffer frameBuffer;
	Scene scene;
	bool colorByNormal;

	void drawRaysToBuffer(Buffer &b, RayGenerator &rg) {
		float maximumValue = 0.0f;
		vector<Vector3> sceneColors;
		unsigned int h = b.getH();
		unsigned int w = b.getW();
		sceneColors.resize(h * w);

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				if (x == 5 && y == 5){
					printf("stuff");
				}
				Ray r = rg.getRayAt(x, y);
				Vector3 c = traceRay(r);
				//c = r.getDirection();
				sceneColors[x + (y * h)] = c;
				
			}

		}

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				Vector3 c = sceneColors[x + (y * h)];
				maximumValue = max(maximumValue, max(c[0], max(c[1], c[2])));
			}

		}

		for (int y = 1; y <= h; y++) {
			for (int x = 0; x < w; x++) {

				Vector3 sceneCol = sceneColors[x + (y - 1) * h];
				sceneCol /= maximumValue;
				sceneCol *= 255.0f;
				Color &buffColor = this->frameBuffer.at(x, h - y);
				buffColor = sceneCol;
			}

		}
		printf("done writing colors\n");
	}

	Vector3 traceRay(Ray &ray) {
		return traceRay(ray, 5);
	}

	Vector3 traceRay(Ray &ray, int recursionDepth) {
		if (recursionDepth >= 10) {
			return Vector3(0, 0, 0);
		}
		this->scene.intersect(ray);
		Hit hit = ray.getFirstHit();
		float t = hit.getParam();

		Vector3 c = Vector3(0, 0, 0);
		if (t > 0) {
			// ray got a hit!

			if (colorByNormal) {
				Vector3 normal = hit.getHitNormal();
				normal *= 255.0f;
				c = Vector3(abs(normal[0]), abs(normal[1]), abs(normal[2]));
			} else {
				c = Vector3(t, t, t);
			}
		}
		return c;

	}

	

	float clamp(float val, float min) {
		if (val < min) {
			return min;
		}
		return val;
	}

};

#endif
