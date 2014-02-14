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

	BoundingBox(Triangle triangle) {
		Vector3 minBound = triangle.getMinBoundaries();
		Vector3 maxBound = triangle.getMaxBoundaries();
		for (int i = 0; i < 3; i++){
			this->minBoundaries[i] = minBound[i];
			this->maxBoundaries[i] = maxBound[i];
		}
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


	virtual Vector3 getCenter() const {
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