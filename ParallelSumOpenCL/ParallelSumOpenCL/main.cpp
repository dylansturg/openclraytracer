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

#define RES 100

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

	IntersectionKernel intersections(width, height, scene, outHits);

	std::cout << "Passed!\n";
	Buffer buffer = Buffer(RES, RES);

	float maxt = 0;
	for (int i = 0; i < width*height; i++){
		if (outHits[i].t < (FLT_MAX - 1.0f)){
			if (outHits[i].t > maxt){
				maxt = outHits[i].t;
			}
		}
		else {
			outHits[i].t = 0;
		}
	}

	cout << "max t: " << maxt << endl;


	for (int y = 0; y < RES; y++)
	{
		for (int x = 0; x < RES; x++)
		{
			float a = (outHits[y*width + x].t / maxt) *255.0f;
			
			Color c = Color(abs(a), abs(a), abs(a));
			if (a <= 0){
				c = Color(255, 0, 0);
			}
			buffer.at(x, RES-y-1) = c;
		}
	}

	simplePPM_write_ppm("ray.ppm", RES, RES, (unsigned char *)&buffer.at(0, 0));

	return SUCCESS;
}

