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
			this->a.s[i] = a.s[i];
			this->b.s[i] = b.s[i];
			this->c.s[i] = c.s[i];
			this->normal.s[i] = normal.s[i];
		}	
		this->materialIndex = materialIndex;

	}

private:
	cl_float3 a;
	cl_float3 b;
	cl_float3 c;
	cl_float3 normal;
	cl_int materialIndex;
};
#endif