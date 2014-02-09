// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <utility>
#include <CL/cl.hpp>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>

#include "AMDHelloWorldSample.h"
#include "CStyleHelloWorld.h"
#include "Scene.h"
#include "objLoader.h"
#include "buffer.h"
#include "simplePPM.h"

#define RES 200

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Scene scene;
	Buffer buffer = Buffer(RES, RES);


	for (int y = 1; y <= RES; y++)
	{
		for (int x = 0; x < RES; x++)
		{
			Color c = Color(x, y, (x + y) % 255);
			buffer.at(x, RES - y) = c;
		}
	}

	simplePPM_write_ppm("ray.ppm", RES, RES, (unsigned char *)&buffer.at(0, 0));

	return CStyleHelloWorld();
}

