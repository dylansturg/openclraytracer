#include "structs.h"

float3 calculateLighting(__global Material* hitMaterial, __global Light* light, __global Material* lightMaterial, __global HitPoint* hitpoint, float3 viewDirection);

__kernel void calculateMaterialColors(__global HitPoint* hitPoints, __global Material* materials, __global Light* lights, int lightSize, __global float* colors, float cameraOriginX, float cameraOriginY, float cameraOriginZ)
{
	int id = get_global_id(0);

	int materialIndex = hitPoints[id].materialId;

	float3 color = (float3) (0,0,0);
    float3 origin = (float3) (cameraOriginX, cameraOriginY, cameraOriginZ);
    if(hitPoints[id].t < (FLT_MAX - 1.0f)){
        for(int i=0; i < lightSize; i++){
             color += calculateLighting(&materials[hitPoints[id].materialId], &lights[i], &materials[lights[i].materialIndex], &hitPoints[id], origin);
         }
    } else{
         color = (float3)(0,0,0);   
    }
    
    colors[3*id+0] = color.x;
    colors[3*id+1] = color.y;
    colors[3*id+2] = color.z;
   

}

float3 calculateLighting(__global Material* hitMaterial, __global Light* light, __global Material* lightMaterial, __global HitPoint* hitpoint, float3 viewDirection)
{
    float3 amb = (float3) (hitMaterial->amb[0], hitMaterial->amb[1], hitMaterial->amb[2]);
    float3 ambInt = (float3) (lightMaterial->amb[0], lightMaterial->amb[1], lightMaterial->amb[2]);
	float3 ambColor = amb*ambInt;

    float3 diff = (float3) (hitMaterial->diff[0], hitMaterial->diff[1], hitMaterial->diff[2]);
    float3 diffInt = (float3) (lightMaterial->diff[0], lightMaterial->diff[1], lightMaterial->diff[2]);

    float3 l = (float3)(light->position[0] - hitpoint->position[0], light->position[1] - hitpoint->position[1], light->position[2] - hitpoint->position[2]);
    float3 hit = (float3)(hitpoint->position[0], hitpoint->position[1], hitpoint->position[2]);
    float3 lightPos = (float3)(light->position[0], light->position[1], light->position[2]);
    float3 n = (float3) (hitpoint->normal[0], hitpoint->normal[1],hitpoint->normal[2]);
    n = normalize(n);
    l = normalize(l);
    l = normalize(lightPos - hit);
    
	float3 diffColor = diff * clamp(dot(n, l), 0.0f, 1.0f) * diffInt;

    float3 spec = (float3) (hitMaterial->spec[0], hitMaterial->spec[1], hitMaterial->spec[2]);
    float3 specInt = (float3) (lightMaterial->spec[0], lightMaterial->spec[1], lightMaterial->spec[2]);
    
    float3 reflect = 2*(dot(l, n))*n - l;
    reflect = normalize(reflect);
    float3 v = normalize(viewDirection - hit);
    float3 specColor = spec * pow(clamp(dot(v, reflect), 0.0f, 1.0f), hitMaterial->shiny) * specInt;
    
    //return ambColor;// + specColor;
}