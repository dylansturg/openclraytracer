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
			this->lightPosition[i] = lightPosition.s[i];
		}
		this->materialID = materialID;
	}

	cl_float* getLightPosition()
	{
		return this->lightPosition;
	}

	cl_int getMaterialID()
	{
		return this->materialID;
	}

	void setMaterialID(cl_int id){
		this->materialID = id;
	}

private:
	cl_float lightPosition[3];
	cl_int materialID;

};


#endif