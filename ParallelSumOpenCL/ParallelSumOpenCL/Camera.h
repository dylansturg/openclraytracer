#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "objLoader.h"
#include "GenVector.h"
#include <CL/cl.h>

class Camera
{
public:
	cl_float3 Pos;
	cl_float3 u;
	cl_float3 v;
	cl_float3 w;
	Camera(){

	}

	Camera(Vector3 initPos, Vector3 lookAt, Vector3 up)
	{
		Vector3 vecW = initPos - lookAt;
		vecW = vecW.normalize();

		Vector3 vecU = up.cross(vecW);
		vecU = vecU.normalize();

		Vector3 vecV = vecW.cross(vecU);
		vecV.normalize();


		for (int i = 0; i < 3; i++)
		{
			this->Pos.s[i] = initPos[i];
			this->u.s[i] = vecU[i];
			this->v.s[i] = vecV[i];
			this->w.s[i] = vecW[i];
		}
	}


private:

};

#endif