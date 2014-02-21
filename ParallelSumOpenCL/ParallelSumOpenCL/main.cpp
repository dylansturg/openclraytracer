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
#include <Windows.h>

#include "CStyleHelloWorld.h"
#include "Scene.h"
#include "objLoader.h"
#include "buffer.h"
#include "simplePPM.h"
#include "ClKernel.h"
#include "HitPoint.h"
#include "IntersectionKernel.h"
#include "MaterialColorKernel.h"
#include "ClRay.h"
#include "BVHTree.h"
#include "CPURayTracer\RayTracer.h"
#include "ContextWrapper.h"

#define RES 2048
#define FRAME_COUNT 1
#define TARGET_DEVICE CL_DEVICE_TYPE_GPU

using namespace std;

long dataUploadTime;
long kernelTimer;
long readTimer;
long overHeadTimer;

int main(int argc, char* argv[])
{
	dataUploadTime = 0;
	kernelTimer = 0;
	readTimer = 0;
	overHeadTimer = 0;


	string extension = ".bi";;
	string triangleFile = "trianglesBinary", materialsFile = "materialsBinary", lightsFile = "lightsBinary", treeFile = "treeBinary", cameraFile = "camBinary";

	Scene parsedScene;
	parsedScene.SceneToBinFiles(triangleFile + extension, materialsFile + extension, lightsFile + extension, treeFile + extension, cameraFile + extension);


	long runTImes[FRAME_COUNT];
	long kernelTimes[FRAME_COUNT];
	long intersectionTimes[FRAME_COUNT];

	Scene scene(triangleFile+extension, materialsFile+extension, lightsFile+extension, treeFile+extension, cameraFile+extension);

	int width = RES;
	int height = RES;

	Vector3 sceneCenter = scene.tree.getNodesList()->at(0).bBox.getCenter();
	Vector3 camCenter = Vector3(scene.camera.Pos.s[0], scene.camera.Pos.s[1], scene.camera.Pos.s[2]);
	Vector3 dif = sceneCenter - camCenter;
	dif *= dif;
	float rotateRadius = sqrt(dif[0] + dif[1] + dif[2]);

	/*
		Camera operations used for rendering rotating scenes with a moving camera.
	*/
	//scene.camera = Camera(Vector3(rotateRadius, 0.05, 0) + sceneCenter, sceneCenter, Vector3(0, 1, 0));
	//scene.cameraLookingAt = sceneCenter;
	//scene.cameraUp = Vector3(0, 1, 0);

	vector<Vector3> outColors;
	vector<HitPoint> outHits;

	ContextWrapper context(TARGET_DEVICE);

	IntersectionKernel intersections(width, height, scene, context);
	MaterialColorKernel materialColors(width, height, scene, context);

	int triangleCount = scene.shapes.size();
	int nodesCount = scene.tree.getNodesList()->size();

	for (int frame = 0; frame < FRAME_COUNT; frame++){
		long startTime = GetTickCount64();

		
		intersections.invokeKernel(width, height, outHits, &(scene.camera));

		intersectionTimes[frame] = GetTickCount64() - startTime;

		materialColors.invokeKernel(width, height, intersections.hitPointsBuffer, outColors, &(scene.camera));
		

		kernelTimes[frame] = GetTickCount64() - startTime;

		Buffer buffer = Buffer(RES, RES);

		float maxt = 0;

		for (int i = 0; i < width*height; i++){
			for (int j = 0; j < 3; j++){
				if (outColors[i][j] < (FLT_MAX - 1.0f)){
					if (outColors[i][j] > maxt){
						maxt = outColors[i][j];
					}
				}
				else {
					outColors[i][j] = 0;
				}
			}
		}


		for (int y = 0; y < RES; y++)
		{
			for (int x = 0; x < RES; x++)
			{
				float a = (outColors[y*width + x][0] / maxt) *255.0f;
				float b = (outColors[y*width + x][1] / maxt) *255.0f;
				float d = (outColors[y*width + x][2] / maxt) *255.0f;

				Color c = Color((a), (b), (d));

				buffer.at(x, RES - y - 1) = c;
			}
		}

		//for (int i = 0; i < width*height; i++){
		//	if (outHits[i].t < (FLT_MAX - 1.0f)){
		//		if (outHits[i].t > maxt){
		//			maxt = outHits[i].t;
		//		}
		//	}
		//	else {
		//		outHits[i].t = 0;
		//	}

		//}

		//for (int y = 0; y < RES; y++)
		//{
		//	for (int x = 0; x < RES; x++)
		//	{
		//		float a = (outHits[y*width + x].t / maxt) *255.0f;
		//		float b = (outHits[y*width + x].normal[1]) *255.0f;
		//		float d = (outHits[y*width + x].normal[2]) *255.0f;


		//		Color c = Color(abs(a), abs(a), abs(a));

		//		buffer.at(x, RES - y - 1) = c;
		//	}
		//}

		char filename[100];
		sprintf_s(filename, "ray_tracer_animation_%d.ppm", frame);

		simplePPM_write_ppm(filename, RES, RES, (unsigned char *)&buffer.at(0, 0));
		Vector3 camPos = Vector3(scene.camera.Pos.s[0], scene.camera.Pos.s[1], scene.camera.Pos.s[2]);
		camPos = sceneCenter + rotateRadius * Vector3(cos(frame * 2 * 3.14 / FRAME_COUNT), 0, sin(frame * 2 * 3.14 / FRAME_COUNT));
		scene.camera = Camera(camPos, scene.cameraLookingAt, scene.cameraUp);

		runTImes[frame] = GetTickCount64() - startTime;
	}

	long sum = 0;

	for (int i = 0; i < FRAME_COUNT; i++)
	{
		cout << "Run time " << i << ": " << runTImes[i] <<  " K time " << kernelTimes[i] << " I time " << intersectionTimes[i] << endl;
		sum += runTImes[i];
	}

	cout << "Average: " << (sum / FRAME_COUNT) << endl;

	clReleaseContext(context.context);

	return SUCCESS;
}

