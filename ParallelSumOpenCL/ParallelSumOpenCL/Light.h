#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "GenVector.h"
#include "CL/cl.h"

class Light
{
public:
	Light()
	{

	}

	Light(cl_float3 lightPosition, int materialID)
	{
		for (int i = 0; i < 3; i++)
		{
			this->lightPosition.s[i] = lightPosition.s[i];
		}
		this->materialID = materialID;
	}

	cl_float3 getLightPosition()
	{
		return this->lightPosition;
	}

	cl_int getMaterialID()
	{
		return this->materialID;
	}

private:
	cl_float3 lightPosition;
	cl_int materialID;

};


#endif