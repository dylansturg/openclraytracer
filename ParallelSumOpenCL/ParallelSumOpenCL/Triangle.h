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

class Triangle{
public:
	Triangle(){}

	Triangle(cl_float3 a, cl_float3 b, cl_float3 c, cl_int materialIndex, cl_float3 normal){
		for (int i = 0; i < 3; i++){
			this->a[i] = a.s[i];
			this->b[i] = b.s[i];
			this->c[i] = c.s[i];
			this->normal[i] = normal.s[i];
		}	
		this->materialIndex = materialIndex;

	}

private:
	cl_float a[3];
	cl_float b[3];
	cl_float c[3];
	cl_float normal[3];
	cl_int materialIndex;
};
#endif