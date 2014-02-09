#pragma once
#ifndef __Scene
#define __Scene

#include "stdafx.h"
#include <utility>
#include <CL/cl.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include "taco.h"
#include "objLoader.h"
#include "Triangle.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "GenVector.h"

class Scene{
public:
	std::vector<Triangle> shapes;
	std::vector<Material> materials;
	std::vector<Light> lights;
	Camera camera;

	Scene(){

		objLoader *objData = new objLoader();
		objData->load("Blue_sphere.obj");

		//create triangles and add

		//Vector3 pos = Vector3();
		//std::vector<LightPoint> lightPoints;

		/*float radius;
		for (int i = 0; i<objData->sphereCount; i++)
		{
		obj_sphere *o = objData->sphereList[i];
		pos = convertToVector(objData->vertexList[o->pos_index]);
		Vector3 radiusVec = convertToVector(objData->normalList[o->up_normal_index]);
		radius = radiusVec.length();
		sphere * tempSphere = new sphere(pos, radius);
		tempSphere->materialIndex = o->material_index;
		shapeHolder.addSphere(tempSphere);

		//printVector(objData->normalList[ o->up_normal_index ]);
		//printVector(objData->normalList[ o->equator_normal_index ]);

		}*/

		//create Triangles and add to new shape holder class or make old class better
		for (int i = 0; i<objData->faceCount; i++)
		{
			obj_face *o = objData->faceList[i];

			cl_float3 a = convertToVector(objData->vertexList[o->vertex_index[0]]);
			cl_float3 b = convertToVector(objData->vertexList[o->vertex_index[1]]);
			cl_float3 c = convertToVector(objData->vertexList[o->vertex_index[2]]);
			cl_float3 normal = convertToVector(objData->normalList[o->vertex_index[0]]); //all vertices same normal
			Triangle tempTriangle(a, b, c, o->material_index, normal);
			shapes.push_back(tempTriangle);

		}

		//create materials
		for (int i = 0; i<objData->materialCount; i++)
		{
			obj_material *m = objData->materialList[i];
			Material tempMaterial = Material(m);
			materials.push_back(m);
		}

		//create lights
		for (int i = 0; i<objData->lightPointCount; i++)
		{
			obj_light_point *o = objData->lightPointList[i];
			Light lightPoint = Light(convertToVector(objData->vertexList[o->pos_index]), o->material_index);
			lights.push_back(lightPoint);
		}


		//create camera
		//obj_vector* initPos = objData->vertexList[ objData->camera->camera_pos_index ];
		Vector3 init = convertToGenVector(objData->vertexList[objData->camera->camera_pos_index]);
		Vector3 look = convertToGenVector(objData->vertexList[objData->camera->camera_look_point_index]);
		Vector3 upVec = convertToGenVector(objData->normalList[objData->camera->camera_up_norm_index]);


		camera = Camera(init, look, upVec);


	}

	cl_float3 convertToVector(obj_vector* convertMe){
		cl_float3 retVal;
		retVal.s[0] = convertMe->e[0];
		retVal.s[1] = convertMe->e[1];
		retVal.s[2] = convertMe->e[2];
		return retVal;
	}

	/*GenVector& normalize()
	{
	T normalizeLength;
	normalizeLength = this->length();

	#ifdef VECTOR_ERROR_CHECKING
	if (normalizeLength <= EPSILON)
	{
	throw(1);
	return *this;
	}
	#endif

	*this /= normalizeLength;
	return *this;
	}*/

	Vector3 convertToGenVector(obj_vector* convertMe){
		return Vector3(convertMe->e[0], convertMe->e[1], convertMe->e[2]);
	}



private:

};
#endif