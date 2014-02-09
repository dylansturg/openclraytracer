#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "objLoader.h"
#include "GenVector.h"
#include "CL/cl.hpp"

class Camera
{
public:
public:
	Camera(){

	}

	Camera(objLoader * objData)
	{
		if (objData->camera != NULL)
		{
			Vector3 Pos = Vector3(objData->vertexList[objData->camera->camera_pos_index]->e[0], objData->vertexList[objData->camera->camera_pos_index]->e[1], objData->vertexList[objData->camera->camera_pos_index]->e[2]);
			Vector3 lookAt = Vector3(objData->vertexList[objData->camera->camera_look_point_index]->e[0], objData->vertexList[objData->camera->camera_look_point_index]->e[1], objData->vertexList[objData->camera->camera_look_point_index]->e[2]);
			Vector3 up = Vector3(objData->normalList[objData->camera->camera_up_norm_index]->e[0], objData->normalList[objData->camera->camera_up_norm_index]->e[1], objData->normalList[objData->camera->camera_up_norm_index]->e[2]);
			setBasis(Pos, lookAt, up);
		}
	}



private:
	cl_float3 Pos;
	cl_float3 u, v, w;

	void setBasis(Vector3 Pos, Vector3 lookAt, Vector3 up)
	{
		Vector3 wTemp = lookAt - Pos;
		Vector3 vecW = -wTemp.normalize();

		Vector3 uTemp = up.cross(this->w.s);
		Vector3 vecU = uTemp.normalize();

		Vector3 vecV = vecW.cross(u.s);

		for (int i = 0; i < 3; i++)
		{
			this->Pos.s[i] = Pos[i];
			this->u.s[i] = vecU[i];
			this->v.s[i] = vecV[i];
			this->w.s[i] = vecW[i];
		}
	}
};

#endif