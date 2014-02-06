typedef struct{
	int one;
	int two;
}someInts;

__kernel void helloworld(__global someInts* in, __global char* out)
{
	int num = get_global_id(0);

	out[num*2] = in[num].one + '0';
	out[num*2+1] = in[num].two + '0';
	if(num == 4)
		out[num*2+2] = 0;

}/* Please Write the OpenCL Kernel(s) code here*/