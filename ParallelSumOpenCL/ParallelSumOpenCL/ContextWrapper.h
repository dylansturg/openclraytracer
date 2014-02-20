#pragma once

#include "stdafx.h"
#include <CL\cl.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

void __stdcall contextErrorCallback(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
	fprintf(stderr, errinfo);
}

class ContextWrapper{
public:
	cl_context context;
	cl_device_id device;

	ContextWrapper(){

	}

	ContextWrapper(long deviceType)
	{
		this->setupContext(deviceType);
	}

	~ContextWrapper(){
		clReleaseContext(this->context);
	}

	void setupContext(long deviceType)
	{
		cl_int err;
		cl_uint numPlatforms;	//the NO. of platforms
		cl_platform_id platform = NULL;	//the chosen platform
		cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
		checkErr(status, "clGetPlatformIDs - count");

		/*For clarity, choose the first available platform. */
		cl_platform_id* platforms;
		if (numPlatforms > 0)
		{
			platforms = (cl_platform_id*)malloc(numPlatforms* sizeof(cl_platform_id));
			status = clGetPlatformIDs(numPlatforms, platforms, NULL);
			checkErr(status, "clGetPlatformIDs");
		}
		else {
			cerr << "Error requesting platforms.  None Available.  That's bad" << endl;
			return;
		}

		/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/

		cl_uint numTargetDevices = 0;
		platform = NULL;
		for (int i = 0; i < numPlatforms; i++){

			status = clGetDeviceIDs(platforms[i], deviceType, 0, NULL, &numTargetDevices);
			if (numTargetDevices > 0){
				platform = platforms[i];
				free(platforms);
				break; // found a valid platform
			}
		}

		if (platform == NULL){
			cerr << "No devices found for available platforms for device type." << endl;
			return;
		}

		cl_device_id* devices = (cl_device_id*)malloc(numTargetDevices*sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, deviceType, numTargetDevices, devices, NULL);
		checkErr(status, "Failed to obtain target devices.");

		/*Step 3: Create context.*/
		this->context = clCreateContext(NULL, 1, devices, contextErrorCallback, NULL, &err);
		checkErr(err, "Failed to create context");

		this->device = devices[0];
		free(devices);
	}


private:

	void checkErr(cl_int err, const char* name){
		if (err != CL_SUCCESS){
			std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
			while (1);
		}
	}

};