#pragma once

#include <CL/cl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>

#include "ContextWrapper.h"


using namespace std;

class ClKernel{
public:
	ClKernel()
	{
		this->clCommandQueue = (cl_command_queue)NULL;
		this->clKernel = (cl_kernel)NULL;
		this->clProgram = (cl_program)NULL;
	}

	ClKernel(string sourceFileName, string kernelFuncName, ContextWrapper context)
	{
		string programSource;
		if(convertToString(sourceFileName, programSource)){
			return;
		}

		cl_int err;
		this->clContext = context;
		/*Step 4: Creating command queue associate with the context.*/
		clCommandQueue = clCreateCommandQueue(context.context, context.device, 0, &err);
		checkErr(err, "Failed to create command queue");

		/*Step 5: Create program object */
		size_t sourceSize[] = {programSource.size()};
		const char* source = programSource.c_str();
		this->clProgram = clCreateProgramWithSource(context.context, 1, &source, sourceSize, &err);
		checkErr(err, "Failed to create program");

		/*Step 6: Build program. */
		err=clBuildProgram(this->clProgram, 1,&(context.device),"-cl-fast-relaxed-math",NULL,NULL);
		if (err != CL_SUCCESS)
		{
			cout << "Program Build failed\n";
			size_t length;
			char buffer[2048];
			clGetProgramBuildInfo(this->clProgram, context.device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &length);
			cout << "--- Build log ---\n " << buffer << endl;
		}
		checkErr(err, "Build program failure");

		this->clKernel = clCreateKernel(this->clProgram,kernelFuncName.c_str(), &err);

	}

	~ClKernel(){
		if (this->clProgram != NULL){
			clReleaseKernel(this->clKernel);
			clFinish(this->clCommandQueue);
			clReleaseCommandQueue(this->clCommandQueue);
			//clReleaseContext(this->clContext);
			clReleaseProgram(this->clProgram);
		}
	}

	ClKernel & operator=(ClKernel & kernel)
	{
		if (this == &kernel)
			return *this;

		if (this->clKernel != NULL){
			clReleaseKernel(this->clKernel);
			clReleaseCommandQueue(this->clCommandQueue);
			clReleaseProgram(this->clProgram);
		}

		this->clCommandQueue = kernel.clCommandQueue;
		this->clContext = kernel.clContext;
		this->clKernel = kernel.clKernel;
		this->clProgram = kernel.clProgram;

		if (kernel.clKernel != NULL){
			kernel.setClNull();
		}

		return *this;
	}

	int setKernelArg(cl_uint argIndex, size_t argSize, const void* argValue){
		return clSetKernelArg(this->clKernel, argIndex, argSize, argValue);
	}

	int runKernel(cl_uint work_dim,
		const size_t * global_work_offset,
		const size_t *global_work_size,
		const size_t *local_work_size = NULL,
		cl_uint num_events_in_wait_list = 0,
		const cl_event *event_wait_list = NULL,
		cl_event *event = NULL){
			return clEnqueueNDRangeKernel(this->clCommandQueue, this->clKernel, work_dim, global_work_offset, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);
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
		return clCreateBuffer(this->clContext.context, flags, size, host_ptr, errcode_ret);
	}

	void static checkErr(cl_int err, const char* name){
		if (err != CL_SUCCESS){
			std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
			while (1);
		}
	}

private:
	cl_kernel clKernel;
	ContextWrapper clContext;
	cl_command_queue clCommandQueue;
	cl_program clProgram;



	void setClNull()
	{
		this->clCommandQueue = NULL;
		this->clKernel = NULL;
		this->clProgram = NULL;
	}


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

};