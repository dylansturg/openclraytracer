#pragma once

#include "ClKernel.h"
#include <vector>
#include "Camera.h"
#include "Triangle.h"
#include "HitPoint.h"
#include "Scene.h"
#include "GenVector.h"

using namespace std;

class MaterialColorKernel{
public:

	MaterialColorKernel(int width, int height, Scene& scene, vector<HitPoint>& hitPoints, vector<Vector3>& outColors){
		vector<float> kernelColors;
		kernelColors.resize(width*height*3);

		cl_int err;
		ClKernel clKernel("MaterialShaderKernel.cl", CL_DEVICE_TYPE_CPU, "calculateMaterialColors");

		vector<Material> materials = scene.materials;
		vector<Light> lights = scene.lights;


		cl_mem hitPointsBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (hitPoints.size()) * sizeof(HitPoint), (void *)&hitPoints[0], &err);
		clKernel.checkErr(err, "creating hit points buffer");


		cl_mem materialsBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (materials.size()) * sizeof(Material), (void *)&materials[0], &err);
		clKernel.checkErr(err, "creating materials buffer");

		cl_mem lightsBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (lights.size()) * sizeof(Light), (void *)&lights[0], &err);
		clKernel.checkErr(err, "creating lights buffer");

		cl_mem colorsBuffer = clKernel.createBuffer(CL_MEM_WRITE_ONLY, (width*height*3) * sizeof(float), NULL, &err);
		clKernel.checkErr(err, "creating output buffer");

		
		//__kernel void calculateMaterialColors(__global HitPoint* hitPoints, __global Material* materials, __global Light* lights, int lightSize, __global float* colors, __global float* cameraOrigin)
		int argIndex = 0;
		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&hitPointsBuffer);
		clKernel.checkErr(err, "setting kernel arg 0");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), &materialsBuffer);
		clKernel.checkErr(err, "setting kernel arg 1");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), &lightsBuffer);
		clKernel.checkErr(err, "setting kernel arg 2");

		int lightCount = lights.size();
		err = clKernel.setKernelArg(argIndex++, sizeof(cl_int), (void *)&lightCount);
		clKernel.checkErr(err, "setting kernel arg 3");

		err = clKernel.setKernelArg(argIndex++, sizeof(cl_mem), (void *)&colorsBuffer);
		clKernel.checkErr(err, "setting kernel arg 3");
		
		float origin[] = { scene.camera.Pos.s[0], scene.camera.Pos.s[1], scene.camera.Pos.s[2] };
		err = clKernel.setKernelArg(argIndex++, sizeof(float), (void *)&origin[0]);
		clKernel.checkErr(err, "setting kernel arg 3");

		err = clKernel.setKernelArg(argIndex++, sizeof(float), (void *)&origin[1]);
		clKernel.checkErr(err, "setting kernel arg 3");

		err = clKernel.setKernelArg(argIndex++, sizeof(float), (void *)&origin[2]);
		clKernel.checkErr(err, "setting kernel arg 3");



		/*Step 10: Running the kernel.*/
		size_t global_work_size[1] = { width*height };
		err = clKernel.runKernel(1, global_work_size, NULL, 0, NULL, NULL);
		clKernel.checkErr(err, "starting the kernel");

		/*Step 11: Read the cout put back to host memory.*/
		
		err = clKernel.readBuffer(colorsBuffer, CL_TRUE, 0, width*height*3 * sizeof(float), (void*)&kernelColors[0], 0, NULL, NULL);
		clKernel.checkErr(err, "starting the read buffer");

		/*Step 12: Clean the resources.*/

		err = clReleaseMemObject(hitPointsBuffer);		//Release mem object.
		err = clReleaseMemObject(materialsBuffer);
		err = clReleaseMemObject(colorsBuffer);


		outColors.resize(width*height);
		for (int i = 0; i < width*height; i++){
			outColors[i] = Vector3(kernelColors[3 * i + 0], kernelColors[3 * i + 1], kernelColors[3 * i + 2]);
		}

	}

private:


};