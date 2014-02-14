#ifndef INTERSECTIONS_H_
#define INTERSECTIONS_H_

#include "structs.h"

int intersectTriangle( HitPoint* hitPoint, Ray * ray, __global Triangle * triangle);
int intersectBoundingBox(HitPoint* hitPoint, Ray* ray, BoundingBox* bBox);
int intersectNode(HitPoint* hitPoint, Ray* ray, __global Node* nodes, int currentIndex, __global Triangle* triangles);
int intersectTree(HitPoint* hitPoint, Ray* ray, __global Node* nodes, int nodeCount, __global Triangle* triangles, int triangleSize);

int intersectBoundingBox(HitPoint* hitPoint, Ray* ray, BoundingBox* bBox)
{
    	float3 direction = ray->direction;
		float3 origin = ray->origin;

		// parameter values when the ray hits x max plane, x min plane, etc.
		// tXMax is not guaranteed to be > tXMin, etc.
		float tXMax, tXMin, tYMax, tYMin, tZMax, tZMin;

		float3 invDirs = ray->inverseDir;

		float xRecip = invDirs.x;
		tXMax = (bBox->maxBoundaries[0] - origin.x) * xRecip;
		tXMin = (bBox->minBoundaries[0] - origin.x) * xRecip;

		float yRecip = invDirs.y;
		tYMax = (bBox->maxBoundaries[1] - origin.y) * yRecip;
		tYMin = (bBox->minBoundaries[1] - origin.y) * yRecip;

		float zRecip = invDirs.z;
		tZMax = (bBox->maxBoundaries[2] - origin.z) * zRecip;
		tZMin = (bBox->minBoundaries[2] - origin.z) * zRecip;

		float minXParam = tXMin < tXMax ? tXMin : tXMax;
		float maxXParam = tXMax > tXMin ? tXMax : tXMin;

		float minYParam = tYMin < tYMax ? tYMin : tYMax;
		float maxYParam = tYMax > tYMin ? tYMax : tYMin;

		float minZParam = tZMin < tZMax ? tZMin : tZMax;
		float maxZParam = tZMax > tZMin ? tZMax : tZMin;

		if (tXMax > 0) {
			if (minYParam < tXMax && maxYParam > tXMax && minZParam < tXMax
				&& maxZParam > tXMax) {
				return 0;
			}
		}

		if (tXMin > 0) {
			if (minYParam < tXMin && maxYParam > tXMin && minZParam < tXMin
				&& maxZParam > tXMin) {
				return 0;
			}
		}

		if (tYMax > 0) {
			if (minXParam < tYMax && maxXParam > tYMax && minZParam < tYMax
				&& maxZParam > tYMax) {
				return 0;
			}
		}

		if (tYMin > 0) {
			if (minXParam < tYMin && maxXParam > tYMin && minZParam < tYMin
				&& maxZParam > tYMin) {
				return 0;
			}
		}

		if (tZMax > 0) {
			if (minXParam < tZMax && maxXParam > tZMax && minYParam < tZMax
				&& maxYParam > tZMax) {
				return 0;
			}
		}

		if (tZMin > 0) {
			if (minXParam < tZMin && maxXParam > tZMin && minYParam < tZMin
				&& maxYParam > tZMin) {
				return 0;
			}
		}
		return -1;
}

void stackPush(NodeStack* stack, int value)
{
    if(stack->top < MAX_STACK)
    {
        stack->indices[stack->top] = value;
        stack->top++;
    }    
}

int stackPop(NodeStack* stack)
{
    if(stack->top >= 0){
        int val = stack->indices[stack->top-1];
        stack->top--;    
        return val;
    }
    return -1;
}

int intersectTree(HitPoint* hitPoint, Ray* ray, __global Node* nodes, int nodeCount, __global Triangle* triangles, int triangleSize)
{
    NodeStack stack;
    stack.top = 0;
    stackPush(&stack, nodeCount-1);
    
    while(stack.top > 0)
    {
        int curIndex = stackPop(&stack);
        if(curIndex < 0){
             return -1;   
        }
        Node curNode = nodes[curIndex];
        
        BoundingBox bBox = curNode.bBox;
    
        if (intersectBoundingBox(hitPoint, ray, &bBox) == 0)
		{
			if (curNode.isLeaf != 1)
			{
               stackPush(&stack, curNode.left);
				if (curNode.right > 0)
				{
                    stackPush(&stack, curNode.right);
				}
			}
			else 
			{
               if(curNode.left < triangleSize){
                   intersectTriangle(hitPoint, ray, &triangles[curNode.left]);
               }
				if (curNode.right >= 0){
                    if(curNode.right < triangleSize){
                        intersectTriangle(hitPoint, ray, &triangles[curNode.right]);
                }
				}
			}
		}  
    }
    return 0;
}

int intersectNode(HitPoint* hitPoint, Ray* ray, __global Node* nodes, int currentIndex, __global Triangle* triangles)
{
    Node curNode = nodes[currentIndex];
    BoundingBox bBox = curNode.bBox;
    
    
    if (intersectBoundingBox(hitPoint, ray, &bBox) == 0)
		{
			if (curNode.isLeaf != 1)
			{
		//		intersectNode(hitPoint, ray, nodes, curNode.left, triangles);
				if (curNode.right > 0)
				{
	//				intersectNode(hitPoint, ray, nodes, curNode.right, triangles);
				}
			}
			else 
			{
//               intersectTriangle(hitPoint, ray, &triangles[curNode.left]);
				if (curNode.right >= 0){
  //                 intersectTriangle(hitPoint, ray, &triangles[curNode.right]);
				}
			}
		}
    return 0;
}


    
int intersectTriangle(HitPoint* hitPoint, Ray * ray, __global Triangle * triangle){
	float a, b, c, d, e, f, g, h, i, j, k, l;
	float ei_minus_hf, gf_minus_di, dh_minus_eg;
	float ak_minus_jb, jc_minus_al, bl_minus_kc;
	float beta, gamma, M;

	float3 A, B, C;
    float3 D, E;
	A = (float3)(triangle->A[0], triangle->A[1], triangle->A[2]);
	B = (float3)(triangle->B[0], triangle->B[1], triangle->B[2]);
	C = (float3)(triangle->C[0], triangle->C[1], triangle->C[2]);
	D = ray->direction;
	E = ray->origin;

	a = A.x - B.x;
	b = A.y - B.y;
	c = A.z - B.z;

	d = A.x - C.x;
	e = A.y - C.y;
	f = A.z - C.z;

	g = D.x;
	h = D.y;
	i = D.z;

	j = A.x - E.x;
	k = A.y - E.y;
	l = A.z - E.z;

	ei_minus_hf = e * i - h * f;
	gf_minus_di = g * f - d * i;
	dh_minus_eg = d * h - e * g;

	// start calculating interesting values
	M = a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg;

	if (M == 0) {
		return -1;
	}

	beta = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / M;

	if (beta < 0 || beta > 1) {
		// half plane test failed
		return -1;
	}

	ak_minus_jb = a * k - j * b;
	jc_minus_al = j * c - a * l;
	bl_minus_kc = b * l - k * c;

	gamma = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / M;

	if (gamma < 0 || gamma > 1) {
		// half plane test failed
		return -1;
	}

	// got a hit.. calculate the parameter

	float t = -(f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / M;
	if (t > 0 && beta + gamma <= 1) {
        if(hitPoint->t > t)
		{
            hitPoint->t = t;
            hitPoint->materialId = triangle->materialId;
            float3 pos = E + D*t;
            hitPoint->position[0] = pos.x;
            hitPoint->position[1] = pos.y;
            hitPoint->position[2] = pos.z;
            
            float3 norm = (float3) (triangle->normal[0], triangle->normal[1],triangle->normal[2]);
            norm = normalize(norm);
            
			hitPoint->normal[0] = norm.x;
			hitPoint->normal[1] = norm.y;
			hitPoint->normal[2] = norm.z;
		    return 0;
       }
    }
	return -1;
}

#endif