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

using namespace std;

const std::string hw("hello world\n");

void checkErr(cl_int err, const char* name){
	if(err != CL_SUCCESS){
		std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
		exit(EXIT_FAILURE);
	}
}

typedef struct{
	cl_int one;
	cl_int two;
}someInts;

int helloWorld(){
	int structCount = 5;
	someInts myInts[5];


	for (int i = 0; i < structCount; i++){
		myInts[i].one = i;
		myInts[i].two = i + 1;
	}


	cl_int err;

	vector<cl::Platform> platformList;

	cl::Platform::get(&platformList);

	checkErr(platformList.size() != 0 ? CL_SUCCESS : -1, "cl::platform::get");

	std::cerr << "Platform number is: " << platformList.size() << std::endl;
	std::string platformVendor;

	platformList[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);

	std::cerr << "Platform is by: " << platformVendor << "\n";

	cl_context_properties cprops[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties) (platformList[1]) (), 0};

	cl::Context context(CL_DEVICE_TYPE_CPU, cprops, NULL, NULL, &err);

	checkErr(err, "Context::Context()");

	char* outH = new char[hw.length() + 5];

	cl::Buffer outCL(
		context,
		CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
		hw.length()+5,
		outH,
		&err);

	cl::Buffer inCL(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(someInts)*structCount, (void*)&myInts, &err);

	checkErr(err, "Buffer::Buffer()");

	vector<cl::Device> devices;

	devices = context.getInfo<CL_CONTEXT_DEVICES>();

	checkErr(devices.size() > 0 ? CL_SUCCESS : -1, "devices.size() > 0");

	std::ifstream file("lesson1_kernels.cl");

	checkErr(file.is_open() ? CL_SUCCESS : -1, "file didn't open");

	std::string prog(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));

	cl::Program::Sources source(1, std::make_pair(prog.c_str(), prog.length()+1));

	cl::Program program(context, source);

	err = program.build(devices, "");

	checkErr(err, "Program::build()");

	cl::Kernel kernel(program, "helloworld", &err);


	checkErr(err, "kernel");
	err = kernel.setArg(0, inCL);
	err = kernel.setArg(1, outCL);
	checkErr(err, "set arg");

	cl::CommandQueue queue(context, devices[0], 0, &err);

	checkErr(err, "command queue");

	cl::Event event;

	err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(structCount), cl::NDRange(1, 1), NULL, &event);

	checkErr(err, "enqueueNDRangeKernel");

	event.wait();

	err = queue.enqueueReadBuffer(outCL, CL_TRUE, 0, hw.length()+1, outH);

	checkErr(err, "CommandQueue::enqueueReadBuffer()");

	std::cout << outH;

	return EXIT_SUCCESS;
}