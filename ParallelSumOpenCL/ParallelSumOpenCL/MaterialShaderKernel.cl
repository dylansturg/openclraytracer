#include "structs.h"

__kernel void calculateMaterialColors(__global HitPoint* hitPoints, __global Material* materials, __global float* colors)
{
	int id = get_global_id(0);


    colors[3*id+0] = 0.0f;
    colors[3*id+1] = 0.0f;
    colors[3*id+2] = 0.0f;
}