#include "structs.h"
#include "ClIntersections.h"
Ray getRay(__global Camera* cam, int x, int y, float w, float h);


__kernel void calculateHitPoints(__global Triangle* triangles, int trianglesSize, __global Camera* camera, int width, int height, __global HitPoint* hitPoints)
{

	int id = get_global_id(0);
	int x = id % width;
	int y = id / width;
	Ray ray = getRay(camera, x, y, (float) width, (float) height);

    HitPoint hitPoint;
    hitPoint.t = FLT_MAX;
    
    for(int i = 0; i < trianglesSize; i++){
        intersect(&hitPoint, &ray, &(triangles[i]));
    }     
    
//    hitPoints[id] = hitPoint;

	hitPoints[id].t = hitPoint.t;
	hitPoints[id].position[0] = hitPoint.position[0];
	hitPoints[id].position[1] = hitPoint.position[1];
	hitPoints[id].position[2] = hitPoint.position[2];

	hitPoints[id].normal[0] = hitPoint.normal[0];
	hitPoints[id].normal[1] = hitPoint.normal[1];
	hitPoints[id].normal[2] = hitPoint.normal[2];

	hitPoints[id].materialId = hitPoint.materialId;
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

    normalize(ray.direction);

	return ray;
}
