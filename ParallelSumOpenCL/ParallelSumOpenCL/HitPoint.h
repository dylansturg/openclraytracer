#pragma once

#include <CL/cl.h>

typedef struct{
	cl_float position[3];
	cl_float normal[3];
	float t;
	int triangleID;
} HitPoint;