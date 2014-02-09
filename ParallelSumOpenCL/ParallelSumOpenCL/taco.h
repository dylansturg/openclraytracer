
#ifndef _TACO_H_
#define _TACO_H_

class taco{
public:
	taco(int salsa, float lettuce[], char cheese){
		this->cheese = cheese;
		this->salsa = salsa;
		for (int i = 0; i < 3; i++){
			this->lettuce.s[i] = lettuce[i];
		}
	}

private:

	cl_float3 lettuce;
	cl_int salsa;
	cl_char cheese;
};

#endif