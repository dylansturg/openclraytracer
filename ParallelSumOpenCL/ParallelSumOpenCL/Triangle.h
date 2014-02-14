#pragma once
#ifndef __Triangle
#define __Triangle

#include "stdafx.h"
#include <utility>
#include <CL/cl.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include "taco.h"
#include "GenVector.h"

class Triangle{
public:
	Triangle(){}

	Triangle(cl_float3 a, cl_float3 b, cl_float3 c, cl_int materialIndex, cl_float3 normal){
		for (int i = 0; i < 3; i++){
			this->a[i] = a.s[i];
			this->b[i] = b.s[i];
			this->c[i] = c.s[i];
		}	
		this->materialIndex = materialIndex;
		calculateNormal();
	}

	Vector3 getMinBoundaries(){
		float minX =
			a[0] < b[0] ?
			(a[0] < c[0] ?
			a[0] : c[0]) :
			(b[0] < c[0] ?
			b[0] : c[0]);
		float minY =
			a[1] < b[1] ?
			(a[1] < c[1] ?
			a[1] : c[1]) :
			(b[1] < c[1] ?
			b[1] : c[1]);
		float minZ =
			a[2] < b[2] ?
			(a[2] < c[2] ?
			a[2] : c[2]) :
			(b[2] < c[2] ?
			b[2] : c[2]);
		return Vector3(minX, minY, minZ);
	}

	Vector3 getMaxBoundaries(){
		float maxX =
			a[0] > b[0] ?
			(a[0] > c[0] ?
			a[0] : c[0]) :
			(b[0] > c[0] ?
			b[0] : c[0]);
		float maxY =
			a[1] > b[1] ?
			(a[1] > c[1] ?
			a[1] : c[1]) :
			(b[1] > c[1] ?
			b[1] : c[1]);

		float maxZ =
			a[2] > b[2] ?
			(a[2] > c[2] ?
			a[2] : c[2]) :
			(b[2] > c[2] ?
			b[2] : c[2]);
		return Vector3(maxX, maxY, maxZ);
	}

	Vector3 getCenter()
	{
		return Vector3((a[0] + b[0] + c[0]) / 3, (a[1] + b[1] + c[1]) / 3, (a[2] + b[2] + c[2]) / 3);
	}

private:
	cl_float a[3];
	cl_float b[3];
	cl_float c[3];
	cl_float normal[3];
	cl_int materialIndex;
	

	void calculateNormal(){
		Vector3 aTemp;
		Vector3 bTemp;
		Vector3 cTemp;
		for (int i = 0; i < 3; i++){
			aTemp.c[i] = a[i];
			bTemp.c[i] = b[i];
			cTemp.c[i] = c[i];
		}

		Vector3 n = (bTemp - aTemp).cross(cTemp - bTemp).normalize();

		for (int i = 0; i < 3; i++){
			this->normal[i] = n.c[i];
		}
	}

};
#endif