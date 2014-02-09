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

	Light(Vector3 lightPosition, int materialID)
	{
		for (int i = 0; i < 3; i++)
		{
			this->lightPosition[i] = lightPosition[i];
		}
		this->materialID = materialID;
	}

	CL_float * getLightPosition()
	{
		return this->lightPosition;
	}

	CL_int getMaterialID()
	{
		return this->materialID;
	}

private:
	CL_float lightPosition[3];
	CL_int materialID;

};


#endif