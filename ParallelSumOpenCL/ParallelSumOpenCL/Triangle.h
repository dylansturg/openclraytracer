#pragma once
#ifndef __Triangle
#define __Triangle

#include "stdafx.h"
#include <utility>
#include <CL/cl.hpp>

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

	Triangle(cl_float a[], cl_float b[], cl_float c[], cl_int materialIndex, cl_float normal[]){
		for (int i = 0; i < 3; i++){
			this->a[i] = a[i];
			this->b[i] = b[i];
			this->c[i] = c[i];
			this->normal[i] = normal[i];
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