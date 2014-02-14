#ifndef __BUFFER
#define __BUFFER
#include "GenVector.h"

class Buffer
{
public:
	Buffer(){

	}

	Buffer(unsigned int w, unsigned int h)
	{
		this->w = w;
		this->h = h;
		alloc();
	}

	Color & at(unsigned int x, unsigned int y)
	{
		return data[x+y*this->w];
	}

	unsigned int getW(){
		return this->w;
	}

	unsigned int getH(){
		return this->h;
	}

private:
	unsigned int w;
	unsigned int h;
	Color * data;

	void alloc()
	{
		data = (Color *)malloc(this->h*this->w*sizeof(Color));
	}
};
#endif
