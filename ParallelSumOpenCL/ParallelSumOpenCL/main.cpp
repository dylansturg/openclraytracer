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

#define RES 1000
#define FRAME_COUNT 1

using namespace std;



int main(int argc, char* argv[])
{
	string extension = "_falcon.bi";;
	string triangleFile = "trianglesBinary", materialsFile = "materialsBinary", lightsFile = "lightsBinary", treeFile = "treeBinary", cameraFile = "camBinary";

	Scene parsedScene;
	parsedScene.SceneToBinFiles(triangleFile + extension, materialsFile + extension, lightsFile + extension, treeFile + extension, cameraFile + extension);


	long runTImes[FRAME_COUNT];

	

	Scene scene(triangleFile+extension, materialsFile+extension, lightsFile+extension, treeFile+extension, cameraFile+extension);

	int width = RES;
	int height = RES;

	vector<Vector3> outColors;
	vector<HitPoint> outHits;
	vector<ClRay> outRays;

	for (int i = 0; i < FRAME_COUNT; i++){
		long startTime = GetTickCount64();

		
		IntersectionKernel intersections(width, height, scene, outHits, outRays);


		
		MaterialColorKernel materialColors(width, height, scene, outHits, outColors);

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
		sprintf_s(filename, "ray_tracer_%d.ppm", i);

		simplePPM_write_ppm(filename, RES, RES, (unsigned char *)&buffer.at(0, 0));
		Vector3 newPos = Vector3(scene.camera.Pos.s[0], scene.camera.Pos.s[1], scene.camera.Pos.s[2]);
		newPos += Vector3(cos(i*2*3.14/FRAME_COUNT), 0, sin(i*2*3.14/FRAME_COUNT));
		scene.camera = Camera(newPos, scene.cameraLookingAt, scene.cameraUp);

		runTImes[i] = GetTickCount64() - startTime;
	}

	long sum = 0;

	for (int i = 0; i < FRAME_COUNT; i++)
	{
		cout << "Run time " << i << ": " << runTImes[i] << endl;
		sum += runTImes[i];
	}

	cout << "Average: " << (sum / FRAME_COUNT) << endl;

	return SUCCESS;
}

