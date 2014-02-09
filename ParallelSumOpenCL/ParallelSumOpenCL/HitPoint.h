#pragma once

#include <CL/cl.h>

class HitPoint{
public:
	cl_float3 position;
	cl_float t;
	cl_int triangleID;


	HitPoint(cl_float3 pos, cl_float t, cl_int triangleID){
		this->position = pos;
		this->t = t;
		this->triangleID = triangleID;
	}

	~HitPoint(){

	}


private:

};