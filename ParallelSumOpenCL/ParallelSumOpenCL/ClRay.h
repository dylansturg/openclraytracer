#pragma once

#include <CL/cl.h>

typedef struct{
	cl_float4 origin;
	cl_float4 direction;
} ClRay;