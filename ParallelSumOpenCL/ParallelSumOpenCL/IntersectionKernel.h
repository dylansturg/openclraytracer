#pragma once

#define MAX_CONCURRENT_KERNELS 10000

#include "ClKernel.h"
#include <vector>
#include "Camera.h"
#include "Triangle.h"
#include "HitPoint.h"
#include "Scene.h"
#include "ClRay.h"
#include <CL/cl.h>
#include "ContextWrapper.h"

using namespace std;

extern long kernelTimer;
extern long readTimer;

class IntersectionKernel{
public:
	ClKernel clKernel;
	cl_mem trianglesBuffer, nodesBuffer, cameraBuffer, hitPointsBuffer;

	IntersectionKernel(int width, int height, Scene& scene, ContextWrapper & context){
		cl_int err;
		this->clKernel = ClKernel("HitPointCalculator.cl", "calculateHitPoints", context);

		vector<Triangle> triangles = scene.shapes;
		vector<BVHNode>* nodes = scene.tree.getNodesList();

		trianglesBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (triangles.size()) * sizeof(Triangle), (void *)&triangles[0], &err);
		clKernel.checkErr(err, "creating triangle buffer");

		nodesBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (nodes->size()) * sizeof(BVHNode), (void *)&(*nodes)[0], &err);
		clKernel.checkErr(err, "creating nodes buffer");


		Camera camera = scene.camera;
		cameraBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (1) * sizeof(Camera), (void *)&camera, &err);
		clKernel.checkErr(err, "creating camera buffer");

		hitPointsBuffer = clKernel.createBuffer(CL_MEM_READ_WRITE, (width*height) * sizeof(HitPoint), NULL, &err);
		clKernel.checkErr(err, "creating output buffer");

		int trianglesSize = triangles.size();

		//__kernel void calculateHitPoints(__global Node* nodes, int nodeCount, __global Triangle* triangles, int trianglesSize, __global Camera* camera, int width, int height, __global HitPoint* hitPoints)

		/*Step 9: Sets Kernel arguments.*/
		int argIndex = 0;

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&nodesBuffer);
		clKernel.checkErr(err,  "setting kernel arg");
		
		int nodeCount = nodes->size();
		err = clKernel.setKernelArg(argIndex++, sizeof(int), &nodeCount);
		clKernel.checkErr(err, "setting kernel arg");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&trianglesBuffer);
		clKernel.checkErr(err, "setting kernel arg");

		err = clKernel.setKernelArg(argIndex++, sizeof(int), &trianglesSize);
		clKernel.checkErr(err, "setting kernel arg");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&cameraBuffer);
		clKernel.checkErr(err, "setting kernel arg");

		err = clKernel.setKernelArg(argIndex++, sizeof(int), &width);
		clKernel.checkErr(err, "setting kernel arg");

		err = clKernel.setKernelArg(argIndex++, sizeof(int), &height);
		clKernel.checkErr(err, "setting kernel arg");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&hitPointsBuffer);
		clKernel.checkErr(err, "setting kernel arg");

		//if (i > requiredKernelExecutions)
		//{
		//	i -= MAX_CONCURRENT_KERNELS;
		//	size_t global_size = (requiredKernelExecutions - i);
		//	size_t offset = i;

		//	err = clKernel.runKernel(1, &offset, &global_size, NULL, 0, NULL, &kernelEvent);
		//	clKernel.checkErr(err, "starting the kernel");

		//	err = clWaitForEvents(1, &kernelEvent);
		//	clKernel.checkErr(err, "kernel execution");
		//}


	}

	~IntersectionKernel()
	{
		cl_int err;
		err = clReleaseMemObject(nodesBuffer);		//Release mem object.
		clKernel.checkErr(err, "freeing args");

		err = clReleaseMemObject(trianglesBuffer);		//Release mem object.
		clKernel.checkErr(err, "freeing args");
		err = clReleaseMemObject(cameraBuffer);
		clKernel.checkErr(err, "freeing args");
		err = clReleaseMemObject(hitPointsBuffer);
		clKernel.checkErr(err, "freeing args");
	}

	bool invokeKernel(int width, int height, vector<HitPoint> & outHits, Camera* sceneCam = NULL)
	{
		cl_event kernelEvent;
		cl_int err;

		if (sceneCam != NULL){
			err = clReleaseMemObject(cameraBuffer);
			clKernel.checkErr(err, "freeing args");
			cameraBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (1) * sizeof(Camera), (void *)sceneCam, &err);
			clKernel.checkErr(err, "creating camera buffer");

			err = clKernel.setKernelArg(4, sizeof(cl_mem), (void *)&cameraBuffer);
			clKernel.checkErr(err, "setting kernel arg 3");
		}

		int requiredKernelExecutions = width*height;
		int i = 0;
		long startTime = GetTickCount64();
		size_t global_work_size = width*height;
		size_t offset = i;
		err = clKernel.runKernel(1, &offset, &global_work_size, NULL, 0, NULL, &kernelEvent);
		clKernel.checkErr(err, "starting the kernel");

		err = clWaitForEvents(1, &kernelEvent);
		clKernel.checkErr(err, "kernel execution");
		//long readStart = GetTickCount64();
		//outHits.resize(width*height);
		//err = clKernel.readBuffer(hitPointsBuffer, CL_TRUE, 0, width*height * sizeof(HitPoint), (void*)&outHits[0], 0, NULL, NULL);
		//clKernel.checkErr(err, "reading buffer");
		//readTimer += GetTickCount64() - readStart;

		kernelTimer += GetTickCount64() - startTime;

		return true;
	}

private:


};