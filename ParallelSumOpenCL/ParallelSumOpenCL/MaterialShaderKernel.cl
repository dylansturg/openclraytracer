#include "structs.h"
#include "ClIntersections.h"

float3 calculateLighting(__global Material* hitMaterial, __global Light* light, __global Material* lightMaterial, HitPoint* hitpoint, float3 viewDirection, __global Triangle* triangles, int triangleSize, __global Node* nodes, int nodeCount);

__kernel void calculateMaterialColors(__global HitPoint* hitPoints, __global Material* materials, __global Light* lights, int lightSize, __global Triangle* triangles, int triangleSize, __global Node* nodeLists, int nodeCount, __global float* colors, __global Camera* camera)
{
	int id = get_global_id(0);

    HitPoint poppedHit;

    poppedHit.t = hitPoints[id].t;
    poppedHit.materialId = hitPoints[id].materialId;
    poppedHit.position[0] = hitPoints[id].position[0];
    poppedHit.position[1] = hitPoints[id].position[1];
    poppedHit.position[2] = hitPoints[id].position[2];

    poppedHit.normal[0] = hitPoints[id].normal[0];
    poppedHit.normal[1] = hitPoints[id].normal[1];
    poppedHit.normal[2] = hitPoints[id].normal[2];

    float3 color = (float3) (0,0,0);
    float3 origin = camera->position;
    int limit = 2;
    float colorScalar = 1.0f;
    int loopCount = 0;
    
    while(poppedHit.t < (FLT_MAX - 1.0f) && limit > 0){
        limit--;
        loopCount++;
        for(int i=0; i < lightSize; i++){
           color += colorScalar * calculateLighting(&materials[poppedHit.materialId], &lights[i], &materials[lights[i].materialIndex], &poppedHit, origin, triangles, triangleSize, nodeLists, nodeCount);
       }

       if(materials[poppedHit.materialId].reflect > 0.0f){
            colorScalar = materials[poppedHit.materialId].reflect;
            Ray reflectRay;
            reflectRay.origin = (float3)(poppedHit.position[0], poppedHit.position[1], poppedHit.position[2]);
            
            float3 view = normalize(origin - reflectRay.origin);
            float3 norm = (float3)(poppedHit.normal[0], poppedHit.normal[1], poppedHit.normal[2]);
            float3 reflectionDir = 2 * (dot(view, norm)) * norm - view;
            
            reflectRay.origin += 0.001f * norm;
            reflectRay.direction = normalize(reflectionDir);
            reflectRay.inverseDir = 1 / reflectRay.direction;

            intersectTree(&poppedHit, &reflectRay, nodeLists, nodeCount, triangles, triangleSize);
            

        } else {
            break;
        }

    }

    colors[3*id+0] = color.x;
    colors[3*id+1] = color.y;
    colors[3*id+2] = color.z;

    return;

}

float3 calculateLighting(__global Material* hitMaterial, __global Light* light, __global Material* lightMaterial, HitPoint* hitpoint, float3 viewDirection, __global Triangle* triangles, int triangleSize, __global Node* nodes, int nodeCount)
{

    float3 hit = (float3)(hitpoint->position[0], hitpoint->position[1], hitpoint->position[2]);
    float3 lightPos = (float3)(light->position[0], light->position[1], light->position[2]);
    float3 n = (float3) (hitpoint->normal[0], hitpoint->normal[1],hitpoint->normal[2]);
    
    n = normalize(n);
    
    float3 amb = (float3) (hitMaterial->amb[0], hitMaterial->amb[1], hitMaterial->amb[2]);
    float3 ambInt = (float3) (lightMaterial->amb[0], lightMaterial->amb[1], lightMaterial->amb[2]);
    float3 ambColor = amb*ambInt;

    Ray lightRay;
    
    lightRay.origin = hit + n*0.01f;
    lightRay.direction = lightPos - lightRay.origin;
    lightRay.direction = normalize(lightRay.direction);
    lightRay.inverseDir = 1 / lightRay.direction;
    
    HitPoint hitPoint;
    hitPoint.t = FLT_MAX;
    hitPoint.materialId = -1;
    
    
    intersectTree(&hitPoint, &lightRay, nodes, nodeCount, triangles, triangleSize);
    if( hitPoint.t < (FLT_MAX - 1.0f)){
        float3 hitPos = (float3)(hitPoint.position[0], hitPoint.position[1], hitPoint.position[2]);
        if(distance(hitPos, lightRay.origin) < distance(lightPos, lightRay.origin)){
            return ambColor;
        }
    }
    
    float3 l = (float3)(light->position[0] - hitpoint->position[0], light->position[1] - hitpoint->position[1], light->position[2] - hitpoint->position[2]);
    
    l = normalize(l);
    l = normalize(lightPos - hit);

    float3 diff = (float3) (hitMaterial->diff[0], hitMaterial->diff[1], hitMaterial->diff[2]);
    float3 diffInt = (float3) (lightMaterial->diff[0], lightMaterial->diff[1], lightMaterial->diff[2]);

    
    
    
    float3 diffColor = diff * clamp(dot(n, l), 0.0f, 1.0f) * diffInt;

    float3 spec = (float3) (hitMaterial->spec[0], hitMaterial->spec[1], hitMaterial->spec[2]);
    float3 specInt = (float3) (lightMaterial->spec[0], lightMaterial->spec[1], lightMaterial->spec[2]);
    
    float3 reflect = 2*(dot(l, n))*n - l;
    reflect = normalize(reflect);
    float3 v = normalize(viewDirection - hit);
    float3 specColor = spec * pow(clamp(dot(v, reflect), 0.0f, 1.0f), hitMaterial->shiny) * specInt;
    
    return ambColor + specColor + diffColor;
}