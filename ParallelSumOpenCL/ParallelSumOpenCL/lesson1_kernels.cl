typedef struct{
	int one;
	int two;
}someInts;

typedef struct{
	int salsa;
	float lettuce[3];
	char cheese;
}tacoStruct;

__kernel void helloworld(__global tacoStruct* in, __global char* out)
{
	int num = get_global_id(0);

	out[0] = in->salsa + '0';
	out[1] = in->lettuce[0];
	out[2] = in->lettuce[1];
	out[3] = in->lettuce[2];
	out[4] = in->cheese;
	out[5] = 0;

}/* Please Write the OpenCL Kernel(s) code here*/