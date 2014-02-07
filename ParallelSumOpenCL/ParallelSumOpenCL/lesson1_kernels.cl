typedef struct{
	int one;
	int two;
}someInts;

typedef struct{
	float3 lettuce;
	int salsa;
	char cheese;
}tacoStruct;

__kernel void helloworld(__global tacoStruct* in, __global char* out)
{
	int num = get_global_id(0);

	out[0] = in->salsa + '0';
	out[1] = in->cheese;
	out[2] = in->lettuce.x;
	out[3] = in->lettuce.y;
	out[4] = in->lettuce.z;
	out[5] = 0;

}/* Please Write the OpenCL Kernel(s) code here*/