#include "structs.h"
Ray getRay(__global Camera* cam, int x, int y, float w, float h);
int intersect( HitPoint* hitPoint, Ray * ray, __global Triangle * triangle, int triangleID);

__kernel void calculateHitPoints(__global Triangle* triangles, int trianglesSize, __global Camera* camera, int width, int height, __global float* hitPoints)
{

	int id = get_global_id(0);
	int x = id % width;
	int y = id / width;
	Ray ray = getRay(camera, x, y, (float) width, (float) height);
    HitPoint hitPoint;
    hitPoint.t = FLT_MAX;
    
    for(int i = 0; i < trianglesSize; i++){
        intersect(&hitPoint, &ray, &(triangles[i]), i);
    }     
    
//    hitPoints[id] = hitPoint;
	hitPoints[id] = hitPoint.t;

}

Ray getRay(__global Camera* cam, int x, int y, float w, float h)
{
	Ray ray;
	ray.origin = cam->position;

	float l, b;
	l = - (w / 2);
	b = - (h / 2);

	float uPos = l + ( w * ((float)x + 0.5)) / w;
	float vPos = b + ( h * ((float)y + 0.5)) / h;

	ray.direction = (-(h/2) * cam->w + uPos * cam->u + vPos * cam->v);
	return ray;
}

int intersect(HitPoint* hitPoint, Ray * ray, __global Triangle * triangle, int triangleID){
	float a, b, c, d, e, f, g, h, i, j, k, l;
	float ei_minus_hf, gf_minus_di, dh_minus_eg;
	float ak_minus_jb, jc_minus_al, bl_minus_kc;
	float beta, gamma, M;

	float3 A, B, C, D, E;
	A = triangle->A;
	B = triangle->B;
	C = triangle->C;
	D = ray->direction;
	E = ray->origin;

	a = A[0] - B[0];
	b = A[1] - B[1];
	c = A[2] - B[2];

	d = A[0] - C[0];
	e = A[1] - C[1];
	f = A[2] - C[2];

	g = D[0];
	h = D[1];
	i = D[2];

	j = A[0] - E[0];
	k = A[1] - E[1];
	l = A[2] - E[2];

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
            hitPoint->triangleID = triangleID;
            hitPoint->position = E + D * t;
		    return 0;
        }
    }
	return -1;

}
