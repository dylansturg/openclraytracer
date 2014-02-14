#ifndef BBVH_BOUNDING_BOX_H
#define BBVH_BOUNDING_BOX_H

#define FAST_BB

#include "Surface.h"
#include "../GenVector.h"
#include <vector>
#include <utility>
#include <sys/time.h>

using namespace std;

class BoundingBox: public Surface {
public:
	BoundingBox() {

	}

	BoundingBox(Surface *surface) {
		this->boundaries[0] = surface->getMinBoundaries();
		this->boundaries[1] = surface->getMaxBoundaries();
	}

	BoundingBox(vector<Surface *>* sceneObjects, int startIndex, int endIndex) {
		int size = endIndex - startIndex;
		Vector3 maxC, minC;
		BoundingBox box = BoundingBox((*sceneObjects)[startIndex]);

		maxC = box.getMaxBoundaries();
		minC = box.getMinBoundaries();

		for (int i = startIndex + 1; i < endIndex; i++) {
			box = BoundingBox((*sceneObjects)[i]);
			for (int j = 0; j < 3; j++) {
				if (maxC[j] < box.getMaxBoundaries()[j]) {
					maxC[j] = box.getMaxBoundaries()[j];
				}
				if (minC[j] > box.getMinBoundaries()[j]) {
					minC[j] = box.getMinBoundaries()[j];
				}
			}
		}

		this->boundaries[1] = maxC;
		this->boundaries[0] = minC;
	}

	~BoundingBox() {

	}

	virtual bool intersect(Ray &ray) {
//		return fast_intersect(ray);
		return slow_intersect(ray);
	}

	virtual bool fast_intersect(Ray &ray) {
		float tXMax, tXMin, tYMax, tYMin, tZMax, tZMin, tMin, tMax;
		Vector3 invDirs = ray.getInvDir();
		int* signs = ray.getDirSigns();
		Vector3 origin = ray.getOrigin();

		tXMin = (this->boundaries[ray.getDirection()[0] < 0 ? 1 : 0][0] - origin[0]) * invDirs[0];
		tXMax = (this->boundaries[ray.getDirection()[0] < 0 ? 0 : 1][0] - origin[0]) * invDirs[0];

		tYMin = (this->boundaries[ray.getDirection()[1] < 0 ? 1 : 0][1] - origin[1]) * invDirs[1];
		tYMax = (this->boundaries[ray.getDirection()[1] < 0 ? 0 : 1][1] - origin[1]) * invDirs[1];

		if ((tXMin > tYMax) || (tYMin > tXMax)) {
			return false;
		}

		tMin = min(tXMin, tYMin);
		tMax = max(tXMax, tYMax);

		tZMin = (this->boundaries[ray.getDirection()[2] < 0 ? 1 : 0][2] - origin[2]) * invDirs[2];
		tZMax = (this->boundaries[ray.getDirection()[2] < 0 ? 0 : 1][2] - origin[2]) * invDirs[2];

		if(tXMin > tXMax || tYMin > tYMax || tZMin > tZMax){
			printf("screwy data: %f, %f, %f, %f, %f, %f\n", tXMax, tXMin, tYMax, tYMin, tZMax, tZMin);
		}

		if((tMin > tZMax) || (tZMin > tMax)){
			return false;
		}

		tMin = min(tMin, tZMin);
		tMax = max(tMax, tZMax);

		float farthest = ray.getFirstHit().getParam();

		return farthest < 0 || tMin < farthest;

	}

	virtual bool slow_intersect(Ray &ray) {
		Vector3 direction = ray.getDirection();
		Vector3 origin = ray.getOrigin();

		// parameter values when the ray hits x max plane, x min plane, etc.
		// tXMax is not guaranteed to be > tXMin, etc.
		float tXMax, tXMin, tYMax, tYMin, tZMax, tZMin;

		Vector3 invDirs = ray.getInvDir();

		float xRecip = invDirs[0];
		tXMax = (this->boundaries[1][0] - origin[0]) * xRecip;
		tXMin = (this->boundaries[0][0] - origin[0]) * xRecip;

		float yRecip = invDirs[1];
		tYMax = (this->boundaries[1][1] - origin[1]) * yRecip;
		tYMin = (this->boundaries[0][1] - origin[1]) * yRecip;

		float zRecip = invDirs[2];
		tZMax = (this->boundaries[1][2] - origin[2]) * zRecip;
		tZMin = (this->boundaries[0][2] - origin[2]) * zRecip;

		float minXParam = tXMin < tXMax ? tXMin : tXMax;
		float maxXParam = tXMax > tXMin ? tXMax : tXMin;

		float minYParam = tYMin < tYMax ? tYMin : tYMax;
		float maxYParam = tYMax > tYMin ? tYMax : tYMin;

		float minZParam = tZMin < tZMax ? tZMin : tZMax;
		float maxZParam = tZMax > tZMin ? tZMax : tZMin;

		if (tXMax > 0) {
			if (minYParam < tXMax && maxYParam > tXMax && minZParam < tXMax
					&& maxZParam > tXMax) {
				return true;
			}
		}

		if (tXMin > 0) {
			if (minYParam < tXMin && maxYParam > tXMin && minZParam < tXMin
					&& maxZParam > tXMin) {
				return true;
			}
		}

		if (tYMax > 0) {
			if (minXParam < tYMax && maxXParam > tYMax && minZParam < tYMax
					&& maxZParam > tYMax) {
				return true;
			}
		}

		if (tYMin > 0) {
			if (minXParam < tYMin && maxXParam > tYMin && minZParam < tYMin
					&& maxZParam > tYMin) {
				return true;
			}
		}

		if (tZMax > 0) {
			if (minXParam < tZMax && maxXParam > tZMax && minYParam < tZMax
					&& maxYParam > tZMax) {
				return true;
			}
		}

		if (tZMin > 0) {
			if (minXParam < tZMin && maxXParam > tZMin && minYParam < tZMin
					&& maxYParam > tZMin) {
				return true;
			}
		}



		return false;
	}

	virtual Vector3 getCenter() const {
		return (boundaries[0] + boundaries[1]) / 2;
	}

	pair<float, int> getSplit() const {
		Vector3 diff = boundaries[1] - boundaries[0];
		int maxIndex = diff.maxComponent();
		Vector3 midPoint = (boundaries[1] + boundaries[0]) / 2;
		float midVal = midPoint[maxIndex];
		return pair<float, int>(midVal, maxIndex);
	}

	virtual Vector3 getMaxBoundaries() const {
		return this->boundaries[1];
	}
	virtual Vector3 getMinBoundaries() const {
		return this->boundaries[0];
	}

private:
	Vector3 boundaries[2];
};

#endif
