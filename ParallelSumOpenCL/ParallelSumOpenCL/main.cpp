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
#include "IntersectionKernel.h"
#include "MaterialColorKernel.h"
#include "ClRay.h"
#define RES 200

using namespace std;



int main(int argc, char* argv[])
{
	Scene scene;

	/*
		HitPoint kernel - calculates hitpoints using triangles (3 vertices) and finds a t, position, and index
		output - Hits and Rays

		Material Kernel - calculates ambient colors using hitpoints and materials
		output - Colors (from material)

		Shading kernel - calculates lighting (ambient, diffuse, specular, and shadows) with color values and lights
		*/


	int width = RES;
	int height = RES;

	vector<HitPoint> outHits;
	vector<ClRay> outRays;
	IntersectionKernel intersections(width, height, scene, outHits, outRays);


	vector<Vector3> outColors;
	MaterialColorKernel materialColors(width, height, scene, outHits, outColors);

	std::cout << "Passed!\n";
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
	//		float a = (outHits[y*width + x].normal[0]) *255.0f;
	//		float b = (outHits[y*width + x].normal[1]) *255.0f;
	//		float d = (outHits[y*width + x].normal[2]) *255.0f;


	//		Color c = Color(abs(a), abs(b), abs(d));

	//		buffer.at(x, RES - y - 1) = c;
	//	}
	//}

	simplePPM_write_ppm("ray_amb2.ppm", RES, RES, (unsigned char *)&buffer.at(0, 0));

	return SUCCESS;
}

