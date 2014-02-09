#ifndef __SIMPLE_PPM_H
#define __SIMPLE_PPM_H

//write ppm
int simplePPM_write_ppm(char const * filename, unsigned int width, unsigned int height, unsigned char const * image);

//read ppm
//the returned pointer is the image data, caller is responseible for freeing the memory
//the width and height are stored in the passed unsigned int locations
unsigned char * simplePPM_read_ppm(char const * filename, unsigned int * width, unsigned int * height);

#endif
