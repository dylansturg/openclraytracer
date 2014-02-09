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

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Scene scene;
	return CStyleHelloWorld();
}

