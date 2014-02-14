#pragma once

#include "../../GenVector.h"
#include <set>
#include "Hit.h"

class Ray {
public:
	Ray() {

	}

	Ray(Vector3 o, Vector3 d) {
		this->origin = o;
		this->direction = (d.normalize());
		this->invDir = Vector3(1 / this->direction[0], 1 / this->direction[1],
				1 / this->direction[2]);
		for (int i = 0; i < 3; i++) {
			this->dirSigns[i] = this->invDir[i] < 0;
		}
	}

	~Ray() {

	}

	Vector3 at(float t) {
		return this->origin + t * this->direction;
	}

	Vector3 getOrigin() {
		return this->origin;
	}
	Vector3 getDirection() {
		return this->direction;
	}

	void setOrigin(Vector3 o) {
		this->origin = o;
	}

	void setDirection(Vector3 d) {
		this->direction = d;
	}

	void addHit(const Hit h) {
		if(this->hit.getParam() < 0 || h.getParam() < this->hit.getParam()){
			hit = h;
		}
	}

	Hit getFirstHit() const {
		return this->hit;
	}

	Vector3 const getInvDir() {
		return this->invDir;
	}

	int* getDirSigns() {
		return &this->dirSigns[0];
	}


private:
	Vector3 origin;
	Vector3 direction;
	Hit hit;
	Vector3 invDir;
	int dirSigns[3];

};
