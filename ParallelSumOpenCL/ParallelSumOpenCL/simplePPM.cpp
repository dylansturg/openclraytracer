#include "simplePPM.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned char simplePPM__getFirstIntChar(FILE * file)
{
	unsigned char c = fgetc(file);
	
	while(c == '\n' || c == '\r' || c == '\t' || c == ' ')
	{
		c = fgetc(file);
		if(c == '#')
		{
			while(c != '\n' && c != '\r')
				c = fgetc(file);
			c = fgetc(file);
		}
	}
	
	return c;
}

unsigned int simplePPM__getuint(FILE * file)
{
	unsigned int intVal = 0;
	char c = 0;
	
	c = simplePPM__getFirstIntChar(file);
	
	if(c < '0' || c > '9')
	{
		printf("Error in PPM format.\n");
		exit(242);
	}
	
	//read int
	while(c >= '0' && c <= '9')
	{
		intVal = intVal*10;
		intVal = intVal + c - '0';
		c = fgetc(file);
	}
	
	if(c == '\n' || c == '\r' || c == '\t' || c == ' ')
		return intVal;
	
	printf("Error in PPM integer format.\n");
	exit(242);
}

FILE* simplePPM__sfopen(char const * name, char const * mode)
{
	FILE *file = NULL;
	file = fopen(name, mode);
	
	if(file == NULL)
	{
		printf("Unable to open file '%s' in mode '%s'.\n", name, mode);
		exit(2);
	}
	
	return file;
}

int simplePPM_write_ppm(char const * filename, unsigned int width, unsigned int height, unsigned char const * image)
{
	FILE *file;
	file = simplePPM__sfopen(filename, "wb");

	fprintf(file, "P6\n%i %i\n%i\n", width, height, 255);
	fwrite(image, 3, width*height, file);
	fclose(file);
	return 0;
}

unsigned char * simplePPM_read_ppm(char const * filename, unsigned int * width, unsigned int * height)
{
	FILE *file;
	file = simplePPM__sfopen(filename, "rb");
	
	int maxVal;
	char asciiMode = 0;
	
	//read header
	char magic[2];
	magic[0] = fgetc(file);
	magic[1] = fgetc(file);
	
	if(strncmp(magic, "P6", 2) == 0)
		asciiMode = 0;
	else if(strncmp(magic, "P3", 2) == 0)
		asciiMode = 1;
	else
	{
		width = 0;
		height = 0;
		printf("File is not a PPM file.\n");
		exit(242);
	}
	
	*width = simplePPM__getuint(file);
	*height = simplePPM__getuint(file);
	
	maxVal = simplePPM__getuint(file);
	if(maxVal > 255)
	{
		printf("This PPM reader cannot parse multi-byte images.\n");
		exit(242);
	}
	
	int size = (*width)*(*height)*3;
	unsigned char * image = (unsigned char *)malloc(size);
	
	//read image
	if(asciiMode)
	{
		for(int i=0; i<size; i++)
		{
			image[i] = simplePPM__getuint(file);
		}
	}
	else
		fread(image, 3, size, file);
	fclose(file);
	return image;
}
