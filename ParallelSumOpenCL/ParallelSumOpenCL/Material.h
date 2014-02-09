#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "GenVector.h"
#include "objLoader.h"
#include "CL/cl.hpp"

class Material
{
public:
	Material(obj_material * mat)
	{
		for (int i = 0; i < 3; i++)
		{
			this->ambient[i] = mat->amb[i];
			this->diffuse[i] = mat->diff[i];
			this->specular[i] = mat->spec[i];
		}

		this->shiny = mat->shiny;
		this->reflect = mat->reflect;
		this->transparency = mat->trans;
		this->glossy = mat->glossy;
	}

	Vector3 getAmbient()
	{
		return this->ambient;
	}

	Vector3 getDiffuse()
	{
		return this->diffuse;
	}

	Vector3 getSpecular()
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
	cl_float ambient[3];
	cl_float diffuse[3];
	cl_float specular[3];
	cl_float shiny;
	cl_float reflect;
	cl_float transparency;
	cl_float glossy;
};

#endif