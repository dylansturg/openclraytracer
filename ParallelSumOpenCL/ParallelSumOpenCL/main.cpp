// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <utility>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>

#include "CStyleHelloWorld.h"
#include "Scene.h"
#include "objLoader.h"
#include "buffer.h"
#include "simplePPM.h"
#include "ClKernel.h"
#include "HitPoint.h"

#define RES 100

using namespace std;

int main(int argc, char* argv[])
{
	Scene scene;



	int width = RES;
	int height = RES;

	cl_int err;
	ClKernel clKernel("HitPointCalculator.cl", CL_DEVICE_TYPE_CPU, "calculateHitPoints");

	vector<Triangle> triangles = scene.shapes;
	/*for (int i = 0; i < 10; i++){

		triangles.push_back(Triangle());

	}*/

	cl_mem trianglesBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (triangles.size()) * sizeof(Triangle), (void *)&triangles[0], &err);
	checkErr(err, "creating triangle buffer");

	Camera camera = scene.camera;
	cl_mem cameraBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (1) * sizeof(Camera), (void *)&camera, &err);
	checkErr(err, "creating camera buffer");

	cl_mem hitPoints = clKernel.createBuffer(CL_MEM_WRITE_ONLY, (width*height) * sizeof(float), NULL, &err);
	checkErr(err, "creating output buffer");

	int trianglesSize = 10;// triangles.size();

	// __kernel void calculateHitPoints(__global Triangle* triangles, int trianglesSize, __global Camera* camera, int width, int height, __global HitPoint* hitPoints)

	/*Step 9: Sets Kernel arguments.*/
	err = clKernel.setKernelArg(0, sizeof(cl_mem), (void *)&trianglesBuffer);
	checkErr(err, "setting kernel arg 0");
	
	err = clKernel.setKernelArg(1, sizeof(int), &trianglesSize);
	checkErr(err, "setting kernel arg 1");
	
	err = clKernel.setKernelArg(2, sizeof(cl_mem), (void *)&cameraBuffer);
	checkErr(err, "setting kernel arg 2");

	err = clKernel.setKernelArg(3, sizeof(int), &width);
	checkErr(err, "setting kernel arg 3");

	err = clKernel.setKernelArg(4, sizeof(int), &height);
	checkErr(err, "setting kernel arg 4");

	err = clKernel.setKernelArg(5, sizeof(cl_mem), (void *)&hitPoints);
	checkErr(err, "setting kernel arg 5");

	/*Step 10: Running the kernel.*/
	size_t global_work_size[1] = { width*height };
	err = clKernel.runKernel(1, global_work_size, NULL, 0, NULL, NULL);

	/*Step 11: Read the cout put back to host memory.*/
	vector<float> output;
	output.resize(width*height);
	//HitPoint* outHits = (HitPoint*)malloc(width*height*sizeof(HitPoint));
	err = clKernel.readBuffer(hitPoints, CL_TRUE, 0, width*height * sizeof(float), (void*)&output[0], 0, NULL, NULL);

	/*Step 12: Clean the resources.*/

	err = clReleaseMemObject(trianglesBuffer);		//Release mem object.
	err = clReleaseMemObject(cameraBuffer);
	err = clReleaseMemObject(hitPoints);

	//free(outHits);

	std::cout << "Passed!\n";
	Buffer buffer = Buffer(RES, RES);

	float maxt = 0;
	for (int i = 0; i < width*height; i++){
		if (output[i] < (FLT_MAX - 1.0f)){
			if (output[i] > maxt){
				maxt = output[i];
			}
		}
		else {
			output[i] = 0;
		}
	}

	cout << "max t: " << maxt << endl;


	for (int y = 1; y <= RES; y++)
	{
		for (int x = 0; x < RES; x++)
		{
			float t = output[(y - 1)*width + x];
			t /= maxt;
			t *= 255.0f;
			Color c = Color((char)t, (char)t, (char)t);
			buffer.at(x, RES - y) = c;
		}
	}

	simplePPM_write_ppm("ray.ppm", RES, RES, (unsigned char *)&buffer.at(0, 0));




	return SUCCESS;
}

