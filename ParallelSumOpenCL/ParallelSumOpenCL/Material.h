#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "GenVector.h"
#include "objLoader.h"
#include <CL/cl.h>

class Material
{
public:
	Material(obj_material * mat)
	{
		for (int i = 0; i < 3; i++)
		{
			this->ambient.s[i] = mat->amb[i];
			this->diffuse.s[i] = mat->diff[i];
			this->specular.s[i] = mat->spec[i];
		}

		this->shiny = mat->shiny;
		this->reflect = mat->reflect;
		this->transparency = mat->trans;
		this->glossy = mat->glossy;
	}

	cl_float3 getAmbient()
	{
		return this->ambient;
	}

	cl_float3 getDiffuse()
	{
		return this->diffuse;
	}

	cl_float3 getSpecular()
	{
		return this->specular;
	}

	double getShiny(){
		return this->shiny;
	}

	double getReflect(){
		return this->reflect;
	}

private:
	cl_float3 ambient;
	cl_float3 diffuse;
	cl_float3 specular;
	cl_float shiny;
	cl_float reflect;
	cl_float transparency;
	cl_float glossy;
};

#endif