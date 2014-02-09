#pragma once

#include <CL/cl.h>
#include <string>
#include <iostream>
#include <fstream>


using namespace std;

class ClKernel{
public:
	ClKernel()
	{

	}

	ClKernel(string sourceFileName, long deviceType, string kernelFuncName)
	{
		string programSource;
		if(convertToString(sourceFileName, programSource)){
			return;
		}

		cl_int err;

		cl_uint numPlatforms;	//the NO. of platforms
		cl_platform_id platform = NULL;	//the chosen platform
		cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
		checkErr(status, "clGetPlatformIDs - count");

		/*For clarity, choose the first available platform. */
		cl_platform_id* platforms;
		if(numPlatforms > 0)
		{
			platforms = (cl_platform_id* )malloc(numPlatforms* sizeof(cl_platform_id));
			status = clGetPlatformIDs(numPlatforms, platforms, NULL);
			checkErr(status, "clGetPlatformIDs");
		} else {
			cerr << "Error requesting platforms.  None Available.  That's bad" << endl;
			return;
		}

		/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/

		cl_uint numTargetDevices = 0;
		platform = NULL;
		for(int i=0; i < numPlatforms; i++){

			status = clGetDeviceIDs(platforms[i], deviceType, 0, NULL, &numTargetDevices);
			if(numTargetDevices > 0){
				platform = platforms[i];
				free(platforms);
				break; // found a valid platform
			}
		}

		if(platform == NULL){
			cerr << "No devices found for available platforms for device type." << endl;
			return;
		}

		cl_device_id* devices = (cl_device_id*)malloc(numTargetDevices*sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, deviceType, numTargetDevices, devices, NULL);
		checkErr(status, "Failed to obtain target devices.");

		/*Step 3: Create context.*/
		clContext = clCreateContext(NULL,1, devices,NULL,NULL,NULL);

		/*Step 4: Creating command queue associate with the context.*/
		clCommandQueue = clCreateCommandQueue(clContext, devices[0], 0, NULL);

		/*Step 5: Create program object */
		size_t sourceSize[] = {programSource.size()};
		const char* source = programSource.c_str();
		cl_program program = clCreateProgramWithSource(clContext, 1, &source, sourceSize, NULL);

		/*Step 6: Build program. */
		status=clBuildProgram(program, 1,devices,NULL,NULL,NULL);

		this->clKernel = clCreateKernel(program,kernelFuncName.c_str(), &err);

		free(devices);
		clReleaseProgram(program);

	}

	~ClKernel(){
		clReleaseKernel(this->clKernel);
		clReleaseCommandQueue(this->clCommandQueue);
		clReleaseContext(this->clContext);
	}

	int setKernelArg(cl_uint argIndex, size_t argSize, const void* argValue){
		return clSetKernelArg(this->clKernel, argIndex, argSize, argValue);
	}

	int runKernel(cl_uint work_dim,
		const size_t *global_work_size,
		const size_t *local_work_size = NULL,
		cl_uint num_events_in_wait_list = 0,
		const cl_event *event_wait_list = NULL,
		cl_event *event = NULL){
			return clEnqueueNDRangeKernel(this->clCommandQueue, this->clKernel, work_dim, NULL, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);
	}

	int readBuffer(cl_mem buffer,
		cl_bool blocking_read,
		size_t offset,
		size_t readSize,
		void *outBufferPtr,
		cl_uint num_events_in_wait_list = 0,
		const cl_event *event_wait_list = NULL,
		cl_event *event = NULL)
	{
		return clEnqueueReadBuffer(this->clCommandQueue, buffer, blocking_read, offset, readSize, outBufferPtr, num_events_in_wait_list, event_wait_list, event);
	}


	cl_mem createBuffer(cl_mem_flags flags,
		size_t size,
		void *host_ptr,
		cl_int *errcode_ret)
	{
		return clCreateBuffer(this->clContext, flags, size, host_ptr, errcode_ret);
	}

private:
	cl_kernel clKernel;
	cl_context clContext;
	cl_command_queue clCommandQueue;


	int convertToString(string filename, string& s)
	{
		size_t size;
		char*  str;
		std::fstream f(filename, (std::fstream::in | std::fstream::binary));

		if(f.is_open())
		{
			size_t fileSize;
			f.seekg(0, std::fstream::end);
			size = fileSize = (size_t)f.tellg();
			f.seekg(0, std::fstream::beg);
			str = new char[size+1];
			if(!str)
			{
				f.close();
				return 0;
			}

			f.read(str, fileSize);
			f.close();
			str[size] = '\0';
			s = str;
			delete[] str;
			return 0;
		}
		cout<<"Error: failed to open file\n:"<<filename<<endl;
		return -1;
	}

	void checkErr(cl_int err, const char* name){
		if(err != CL_SUCCESS){
			std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

};