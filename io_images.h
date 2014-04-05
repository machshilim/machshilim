/**
Machshilim code for reading/writing grayscale png images
*/
#ifndef IO_IMAGES_H
#define IO_IMAGES_H

/**
Load a PNG image from the hard drive. The function dynamically allocates memory for the image,
it must be freed after using it!!!

Input parameters:
	filenameIn - String containing the path of the file on the hard drive (file location).
	image - Pointer to the array where the image will be kept.
	width - Pointer to width of the image.
	height - Pointer to height of the image.
*/ 
void loadImage(const char* filenameIn, unsigned char** image, unsigned int* width, unsigned int* height);

/**
Write a black&white PNG image to the hard drive. 

Input parameters:
	filenameOut - String containing the path where the file will be saved on the hard drive.
	image - Array containing the image contents. This is the data written to the disk.
	width - Width of the image.
	height - Height of the image.
*/ 
void writeImage(const char* filenameOut, const unsigned char* image, unsigned int width, unsigned int height);

#endif