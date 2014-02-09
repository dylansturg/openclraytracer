#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "GenVector.h"

class Buffer
{
public:
	Buffer()
	{

	}

	Buffer(unsigned int w, unsigned int h)
	{
		this->h = h;
		this->w = w;
		alloc();
	}

	Color & at(unsigned int x, unsigned int y)
	{
		return data[x + y*this->w];
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