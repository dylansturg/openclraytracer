#ifndef BBVH_BOUNDING_BOX_H
#define BBVH_BOUNDING_BOX_H

#define FAST_BB

#include "GenVector.h"
#include <vector>
#include <utility>
#include "Triangle.h"

using namespace std;

class BoundingBox {
public:
	BoundingBox() {

	}

	BoundingBox(vector<Triangle>* sceneObjects, int startIndex, int endIndex) {
		int size = endIndex - startIndex;
		Vector3 maxC, minC;

		maxC = (*sceneObjects)[startIndex].getMaxBoundaries();
		minC = (*sceneObjects)[startIndex].getMinBoundaries();

		for (int i = startIndex + 1; i < endIndex; i++) {
			Vector3 lmax, lmin;
			lmax = (*sceneObjects)[i].getMaxBoundaries();
			lmin = (*sceneObjects)[i].getMinBoundaries();

			for (int j = 0; j < 3; j++) {
				if (maxC[j] < lmax[j]) {
					maxC[j] = lmax[j];
				}
				if (minC[j] > lmin[j]) {
					minC[j] = lmin[j];
				}
			}
		}

		for (int i = 0; i < 3; i++){
			this->maxBoundaries[i] = maxC[i];
			this->minBoundaries[i] = minC[i];
		}
	}

	~BoundingBox() {

	}

	bool intersect(Ray &ray) {
		Vector3 direction = ray.getDirection();
		Vector3 origin = ray.getOrigin();

		// parameter values when the ray hits x max plane, x min plane, etc.
		// tXMax is not guaranteed to be > tXMin, etc.
		float tXMax, tXMin, tYMax, tYMin, tZMax, tZMin;

		Vector3 invDirs = ray.getInvDir();

		float xRecip = invDirs[0];
		tXMax = (this->maxBoundaries[0] - origin[0]) * xRecip;
		tXMin = (this->minBoundaries[0] - origin[0]) * xRecip;

		float yRecip = invDirs[1];
		tYMax = (this->maxBoundaries[1] - origin[1]) * yRecip;
		tYMin = (this->minBoundaries[1] - origin[1]) * yRecip;

		float zRecip = invDirs[2];
		tZMax = (this->maxBoundaries[2] - origin[2]) * zRecip;
		tZMin = (this->minBoundaries[2] - origin[2]) * zRecip;

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


	Vector3 getCenter() const {
		Vector3 min = Vector3(minBoundaries[0], minBoundaries[1], minBoundaries[2]);
		Vector3 max = Vector3(maxBoundaries[0], maxBoundaries[1], maxBoundaries[2]);

		return (min + max) / 2;
	}

	pair<float, int> getSplit() const {
		Vector3 min = Vector3(minBoundaries[0], minBoundaries[1], minBoundaries[2]);
		Vector3 max = Vector3(maxBoundaries[0], maxBoundaries[1], maxBoundaries[2]);

		Vector3 diff = max - min;
		int maxIndex = diff.maxComponent();
		Vector3 midPoint = (max + min) / 2;
		float midVal = midPoint[maxIndex];
		return pair<float, int>(midVal, maxIndex);
	}

private:
	cl_float minBoundaries[3];
	cl_float maxBoundaries[3];
};

#endif