/**********************************************************************
Copyright ©2013 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

// For clarity,error checking has been omitted.

#include <CL/cl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include "ClKernel.h"



#define SUCCESS 0
#define FAILURE 1

using namespace std;

/* convert the kernel file into a string */
int convertToString(const char *filename, std::string& s)
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
	return FAILURE;
}

void checkErr(cl_int err, const char* name){
	if (err != CL_SUCCESS){
		std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int CStyleHelloWorld()
{
	cl_int err;
	string hw("Hello World!");
	ClKernel clKernel("HelloWorld_Kernel.cl", "helloworld", (ContextWrapper)NULL);


	cl_mem inputBuffer = clKernel.createBuffer(CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, (hw.size() + 1) * sizeof(char),(void *) hw.c_str(), &err);
	checkErr(err, "creating input buffer");
	cl_mem outputBuffer = clKernel.createBuffer(CL_MEM_WRITE_ONLY , (hw.size() + 1) * sizeof(char), NULL, &err);
	checkErr(err, "creating output buffer");

	/*Step 9: Sets Kernel arguments.*/
	err = clKernel.setKernelArg(0, sizeof(cl_mem), (void *)&inputBuffer);
	checkErr(err, "setting kernel arg 0");
	err = clKernel.setKernelArg(1, sizeof(cl_mem), (void *)&outputBuffer);
	checkErr(err, "setting kernel arg 1");
	
	/*Step 10: Running the kernel.*/
	size_t global_work_size[1] = {hw.size()};
	err = clKernel.runKernel(1, global_work_size, NULL, 0, NULL, NULL);

	/*Step 11: Read the cout put back to host memory.*/
	char* output = (char*)malloc((hw.size() + 1)*sizeof(char));
	err = clKernel.readBuffer(outputBuffer, CL_TRUE, 0, hw.size() * sizeof(char), output, 0, NULL, NULL);
	
	output[hw.size()] = '\0';	//Add the terminal character to the end of output.
	cout << "\noutput string:" << endl;
	cout << output << endl;

	/*Step 12: Clean the resources.*/

	err = clReleaseMemObject(inputBuffer);		//Release mem object.
	err = clReleaseMemObject(outputBuffer);


	if (output != NULL)
	{
		free(output);
		output = NULL;
	}

	std::cout<<"Passed!\n";
	return SUCCESS;
}