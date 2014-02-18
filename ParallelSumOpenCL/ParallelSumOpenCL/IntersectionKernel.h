#pragma once

#include "ClKernel.h"
#include <vector>
#include "Camera.h"
#include "Triangle.h"
#include "HitPoint.h"
#include "Scene.h"
#include "ClRay.h"

using namespace std;

class IntersectionKernel{
public:

	IntersectionKernel(int width, int height, Scene& scene, vector<HitPoint>& outHits, vector<ClRay>& outRays){

		cl_int err;
		ClKernel clKernel("HitPointCalculator.cl", CL_DEVICE_TYPE_GPU, "calculateHitPoints");

		vector<Triangle> triangles = scene.shapes;
		vector<BVHNode>* nodes = scene.tree.getNodesList();

		cl_mem trianglesBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (triangles.size()) * sizeof(Triangle), (void *)&triangles[0], &err);
		clKernel.checkErr(err, "creating triangle buffer");

		cl_mem nodesBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (nodes->size()) * sizeof(BVHNode), (void *)&(*nodes)[0], &err);
		clKernel.checkErr(err, "creating nodes buffer");


		Camera camera = scene.camera;
		cl_mem cameraBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (1) * sizeof(Camera), (void *)&camera, &err);
		clKernel.checkErr(err, "creating camera buffer");

		cl_mem hitPoints = clKernel.createBuffer(CL_MEM_WRITE_ONLY, (width*height) * sizeof(HitPoint), NULL, &err);
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

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&hitPoints);
		clKernel.checkErr(err, "setting kernel arg");

		cl_event kernelEvent;

		size_t global_work_size[1] = { width*height };
		err = clKernel.runKernel(1, global_work_size, NULL, 0, NULL, &kernelEvent);
		clKernel.checkErr(err, "starting the kernel");

		err = clWaitForEvents(1, &kernelEvent);
		clKernel.checkErr(err, "kernel execution");


		/*Step 11: Read the cout put back to host memory.*/
		outHits.resize(width*height);
		err = clKernel.readBuffer(hitPoints, CL_TRUE, 0, width*height * sizeof(HitPoint), (void*)&outHits[0], 0, NULL, NULL);
		clKernel.checkErr(err, "reading buffer");

		/*Step 12: Clean the resources.*/

		err = clReleaseMemObject(nodesBuffer);		//Release mem object.
		clKernel.checkErr(err, "freeing args");

		err = clReleaseMemObject(trianglesBuffer);		//Release mem object.
		clKernel.checkErr(err, "freeing args");
		err = clReleaseMemObject(cameraBuffer);
		clKernel.checkErr(err, "freeing args");
		err = clReleaseMemObject(hitPoints);
		clKernel.checkErr(err, "freeing args");
	}

private:


};