/*
* SortComparators.h
*
*  Created on: Jan 10, 2014
*      Author: dylan
*/

#ifndef SORTCOMPARATORS_H_
#define SORTCOMPARATORS_H_

#include "Triangle.h"

bool compareXAxis(Triangle triangle1, Triangle triangle2){
	return triangle1.getCenter()[0] < triangle2.getCenter()[0];
}

bool compareYAxis(Triangle triangle1, Triangle triangle2){
	return triangle1.getCenter()[1] < triangle2.getCenter()[1];
}

bool compareZAxis(Triangle triangle1, Triangle triangle2){
	return triangle1.getCenter()[2] < triangle2.getCenter()[2];
}

typedef bool(*comparator) (Triangle, Triangle);

static comparator comparators[] = { compareXAxis, compareYAxis, compareZAxis };


#endif /* SORTCOMPARATORS_H_ */