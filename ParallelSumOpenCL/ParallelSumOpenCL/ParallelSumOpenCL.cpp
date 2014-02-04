// ParallelSumOpenCL.cpp : Defines the entry point for the console application.
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

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	return helloWorld();
}

