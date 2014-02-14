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
#include "CPURayTracer\Rays\Ray.h"
#include "BVHTree.h"

class Scene{
public:
	std::vector<Triangle> shapes;
	std::vector<Material> materials;
	std::vector<Light> lights;
	Camera camera;
	BVHTree tree;

	Scene(){

		objLoader *objData = new objLoader();
		objData->load("cornell_box.obj");

		//create Triangles
		for (int i = 0; i<objData->faceCount; i++)
		{
			obj_face *o = objData->faceList[i];

			cl_float3 a = convertToVector(objData->vertexList[o->vertex_index[0]]);
			cl_float3 b = convertToVector(objData->vertexList[o->vertex_index[1]]);
			cl_float3 c = convertToVector(objData->vertexList[o->vertex_index[2]]);
			cl_float3 normal;
			normal.s[0] = 0;
			//if (o->normal_index >= 0){
			//	normal = convertToVector(objData->normalList[o->normal_index[0]]); //all vertices same normal
			//}
			shapes.push_back(Triangle(a, b, c, o->material_index, normal));

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
			lights.push_back(Light(convertToVector(objData->vertexList[o->pos_index]), o->material_index));
		}


		//create camera
		Vector3 init = convertToGenVector(objData->vertexList[objData->camera->camera_pos_index]);
		Vector3 look = convertToGenVector(objData->vertexList[objData->camera->camera_look_point_index]);
		Vector3 upVec = convertToGenVector(objData->normalList[objData->camera->camera_up_norm_index]);


		camera = Camera(init, look, upVec);

		delete objData;

		this->tree = BVHTree(this->shapes);
	}

	cl_float3 convertToVector(obj_vector* convertMe){
		cl_float3 retVal;
		retVal.s[0] = convertMe->e[0];
		retVal.s[1] = convertMe->e[1];
		retVal.s[2] = convertMe->e[2];
		return retVal;
	}

	Vector3 convertToGenVector(obj_vector* convertMe){
		return Vector3(convertMe->e[0], convertMe->e[1], convertMe->e[2]);
	}

	void intersect(Ray& ray){
		this->tree.intersect(ray);
	}


private:

};
#endif