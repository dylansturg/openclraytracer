#pragma once

#include "ClKernel.h"
#include <vector>
#include "Camera.h"
#include "Triangle.h"
#include "HitPoint.h"
#include "Scene.h"
#include "GenVector.h"

using namespace std;

extern long kernelTimer;
extern long dataUploadTime;

class MaterialColorKernel{
public:
	ClKernel clKernel;
	cl_mem  trianglesBuffer, nodesBuffer, materialsBuffer, lightsBuffer, colorsBuffer, cameraBuffer;

	MaterialColorKernel(int width, int height, Scene& scene, ContextWrapper & context){
		cl_int err;
		this->clKernel = ClKernel("MaterialShaderKernel.cl", "calculateMaterialColors", context);

		vector<Material> materials = scene.materials;
		vector<Light> lights = scene.lights;
		vector<Triangle> triangles = scene.shapes;
		vector<BVHNode>* nodes = scene.tree.getNodesList();

		trianglesBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (triangles.size()) * sizeof(Triangle), (void *)&triangles[0], &err);
		clKernel.checkErr(err, "creating triangles buffer");

		nodesBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (nodes->size()) * sizeof(BVHNode), (void *)&(*nodes)[0], &err);
		clKernel.checkErr(err, "creating nodes buffer");

		materialsBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (materials.size()) * sizeof(Material), (void *)&materials[0], &err);
		clKernel.checkErr(err, "creating materials buffer");

		lightsBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (lights.size()) * sizeof(Light), (void *)&lights[0], &err);
		clKernel.checkErr(err, "creating lights buffer");

		colorsBuffer = clKernel.createBuffer(CL_MEM_WRITE_ONLY, (width*height * 3) * sizeof(float), NULL, &err);
		clKernel.checkErr(err, "creating output buffer");

		Camera camera = scene.camera;
		cameraBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (1) * sizeof(Camera), (void *)&camera, &err);
		clKernel.checkErr(err, "creating camera buffer");

		//__kernel void calculateMaterialColors(__global HitPoint* hitPoints, __global Material* materials, __global Light* lights, int lightSize, __global Triangle* triangles, int triangleSize, __global Node* nodeLists, int nodeCount, __global float* colors, float cameraOriginX, float cameraOriginY, float cameraOriginZ)
		int argIndex = 1;
		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), &materialsBuffer);
		clKernel.checkErr(err, "setting kernel arg 1");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), &lightsBuffer);
		clKernel.checkErr(err, "setting kernel arg 2");

		int lightCount = lights.size();
		err = clKernel.setKernelArg(argIndex++, sizeof(cl_int), (void *)&lightCount);
		clKernel.checkErr(err, "setting kernel arg 3");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&trianglesBuffer);
		clKernel.checkErr(err, "setting kernel arg 3");

		int triangleCount = triangles.size();
		err = clKernel.setKernelArg(argIndex++, sizeof(cl_int), (void *)&triangleCount);
		clKernel.checkErr(err, "setting kernel arg 3");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&nodesBuffer);
		clKernel.checkErr(err, "setting kernel arg 3");

		int nodesCount = nodes->size();
		err = clKernel.setKernelArg(argIndex++, sizeof(cl_int), (void *)&nodesCount);
		clKernel.checkErr(err, "setting kernel arg 3");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&colorsBuffer);
		clKernel.checkErr(err, "setting kernel arg 3");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&cameraBuffer);
		clKernel.checkErr(err, "setting kernel arg 3");

	}

	~MaterialColorKernel(){
		/*Step 12: Clean the resources.*/
		cl_int err;
		err = clReleaseMemObject(materialsBuffer);
		clKernel.checkErr(err, "failed to free buffer");
		err = clReleaseMemObject(colorsBuffer);
		clKernel.checkErr(err, "failed to free buffer");
		err = clReleaseMemObject(lightsBuffer);
		clKernel.checkErr(err, "failed to free buffer");
		err = clReleaseMemObject(trianglesBuffer);
		clKernel.checkErr(err, "failed to free buffer");
		err = clReleaseMemObject(nodesBuffer);
		clKernel.checkErr(err, "failed to free buffer");
	}

	bool invokeKernel(int width, int height, cl_mem hitPointsBuffer, vector<Vector3> & outColors, Camera* sceneCam = NULL)
	{
		cl_event kernelEvent;
		cl_int err;
		cl_mem hitpoints = hitPointsBuffer;

		long startTime = GetTickCount64();

		//hitPointsBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (hitPoints.size()) * sizeof(HitPoint), (void *)&hitPoints[0], &err);
		//clKernel.checkErr(err, "creating hit points buffer");

		if (sceneCam != NULL){
			err = clReleaseMemObject(cameraBuffer);
			clKernel.checkErr(err, "freeing args");
			cameraBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (1) * sizeof(Camera), (void *)sceneCam, &err);
			clKernel.checkErr(err, "creating camera buffer");

			err = clKernel.setKernelArg(9, sizeof(cl_mem), (void *)&cameraBuffer);
			clKernel.checkErr(err, "setting kernel arg 3");
		}

		err = clKernel.setKernelArg(0, sizeof(cl_mem), (void *)&hitpoints);
		clKernel.checkErr(err, "setting kernel arg 0");

		//dataUploadTime += GetTickCount64() - startTime;

		int requiredKernelExecutions = width*height;
		int i = 0;
		size_t global_work_size = width*height;
		size_t offset = i;
		err = clKernel.runKernel(1, &offset, &global_work_size, NULL, 0, NULL, &kernelEvent);
		clKernel.checkErr(err, "starting the kernel");

		err = clWaitForEvents(1, &kernelEvent);
		clKernel.checkErr(err, "kernel execution");

		outColors.resize(width*height);
		err = clKernel.readBuffer(colorsBuffer, CL_TRUE, 0, width*height * 3 * sizeof(float), (void*)&outColors[0], 0, NULL, NULL);
		clKernel.checkErr(err, "starting the read buffer");

		kernelTimer += GetTickCount64() - startTime;

		return true;
	}

private:


};