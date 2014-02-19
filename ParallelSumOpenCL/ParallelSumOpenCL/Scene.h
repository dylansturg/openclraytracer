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

using namespace std;

class Scene{
public:
	std::vector<Triangle> shapes;
	std::vector<Material> materials;
	std::vector<Light> lights;
	Camera camera;
	BVHTree tree;
	Vector3 cameraUp, cameraLookingAt;

	Scene(string binaryTrianglesFile, string binaryMaterialsFile, string binaryLightsFile, string binaryTreeFile, string binaryCameraFile)
	{
		fstream trianglesBinary(binaryTrianglesFile, ios::in | ios::binary);
		this->shapes.empty();
		int index = 0;
		while (!trianglesBinary.eof()){
			this->shapes.push_back(Triangle());
			trianglesBinary.read((char*)(&this->shapes[index]), sizeof(Triangle));
			index++;
		}
		this->shapes.pop_back(); // last one is extra, due to file reading junk
		trianglesBinary.close();

		fstream materialsBinary(binaryMaterialsFile, ios::in | ios::binary);
		this->materials.empty();
		index = 0;
		while (!materialsBinary.eof()){
			this->materials.push_back(Material());
			materialsBinary.read((char*)(&this->materials[index]), sizeof(Material));
			index++;
		}
		this->materials.pop_back();
		materialsBinary.close();

		fstream lightsBinary(binaryLightsFile, ios::in | ios::binary);
		this->lights.empty();
		index = 0;
		while (!lightsBinary.eof()){
			this->lights.push_back(Light());
			lightsBinary.read((char*)(&this->lights[index]), sizeof(Light));
			index++;
		}
		this->lights.pop_back();
		lightsBinary.close();


		fstream treeBinary(binaryTreeFile, ios::in | ios::binary);
		vector<BVHNode>* treeNodes = new vector<BVHNode>();
		index = 0;
		while (!treeBinary.eof()){
			treeNodes->push_back(BVHNode());
			treeBinary.read((char*)(&(*treeNodes)[index]), sizeof(BVHNode));
			index++;
		}
		treeNodes->pop_back();
		treeBinary.close();

		this->tree = BVHTree(treeNodes);

		fstream cameraBinary(binaryCameraFile, ios::in | ios::binary);
		cameraBinary.read((char*)&(this->camera), sizeof(Camera));

		cameraBinary.read((char*)&(this->cameraLookingAt), sizeof(Vector3));
		cameraBinary.read((char*)&(this->cameraUp), sizeof(Vector3));

		cameraBinary.close();


	}

	Scene(){

		objLoader *objData = new objLoader();
		objData->load("dragon.obj");

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

		if (this->materials.size() == 0)
		{
			this->materials.push_back(Material());

			// make all the triangles use the default instead...
			for (int i = 0; i < this->shapes.size(); i++)
			{
				if (this->shapes[i].materialIndex < 0)
					this->shapes[i].materialIndex = 0;
			}

			for (int i = 0; i < this->lights.size(); i++)
			{
				if (this->lights[i].getMaterialID() < 0){
					this->lights[i].setMaterialID(0);
				}
			}
		}


		//create camera
		Vector3 init = convertToGenVector(objData->vertexList[objData->camera->camera_pos_index]);
		Vector3 look = convertToGenVector(objData->vertexList[objData->camera->camera_look_point_index]);
		Vector3 upVec = convertToGenVector(objData->normalList[objData->camera->camera_up_norm_index]);

		this->cameraLookingAt = look;
		this->cameraUp = upVec;

		camera = Camera(init, look, upVec);

		delete objData;

		this->tree = BVHTree(this->shapes);
		BVHNode node = this->tree.getNodesList()->at(0);
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
		this->tree.intersectTree(&ray, this->shapes);
	}


	void SceneToBinFiles(string binaryTrianglesFile, string binaryMaterialsFile, string binaryLightsFile, string binaryTreeFile, string binaryCameraFile){
		fstream trianglesOut(binaryTrianglesFile, ios::binary | ios::out);
		
		for (int i = 0; i < this->shapes.size(); i++){
			trianglesOut.write((char*)(&this->shapes[i]), sizeof(Triangle));
		}

		trianglesOut.close();

		fstream materialsOut(binaryMaterialsFile, ios::binary | ios::out);
		for (int i = 0; i < this->materials.size(); i++){
			materialsOut.write((char*)(&this->materials[i]), sizeof(Material));
		}
		materialsOut.close();

		fstream lightsOut(binaryLightsFile, ios::binary | ios::out);
		for (int i = 0; i < this->lights.size(); i++){
			lightsOut.write((char*)(&this->lights[i]), sizeof(Light));
		}
		lightsOut.close();

		fstream treeOut(binaryTreeFile, ios::binary | ios::out);
		vector<BVHNode>* tree = this->tree.getNodesList();
		for (int i = 0; i < tree->size(); i++){
			treeOut.write((char*)(&(*tree)[i]), sizeof(BVHNode));
		}
		treeOut.close();

		fstream cameraOut(binaryCameraFile, ios::binary | ios::out);
		cameraOut.write((char*)&(this->camera), sizeof(Camera));
		cameraOut.write((char*)&(this->cameraLookingAt), sizeof(Vector3));
		cameraOut.write((char*)&(this->cameraUp), sizeof(Vector3));
		cameraOut.close();

		return;
	}

private:

};
#endif