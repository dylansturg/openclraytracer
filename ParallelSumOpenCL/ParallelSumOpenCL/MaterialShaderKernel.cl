#include "structs.h"

__kernel void calculateMaterialColors(__global HitPoint* hitPoints, __global Material* materials, __global float* colors)
{
	int id = get_global_id(0);

	int materialIndex = hitPoints[id].materialId;

	float3 ambient;
    if(hitPoints[id].t < (FLT_MAX - 1.0f)){
         ambient = materials[materialIndex].amb;
    } else{
         ambient = (float3)(0,0,0);   
    }
    colors[3*id+0] = ambient[0];
    colors[3*id+1] = ambient[1];
    colors[3*id+2] = ambient[2];
}