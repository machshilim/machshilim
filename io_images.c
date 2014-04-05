#include "lodepng.h"
#include "io_images.h"
#include <stdio.h>

/**
Load a black&white PNG image from the hard drive. The function dynamically allocates memory for the image,
it must be freed after using it!!!

Input parameters:
	filenameIn - String containing the path of the file on the hard drive (file location).
	image - Pointer to the array where the image will be kept.
	width - Pointer to width of the image.
	height - Pointer to height of the image.
*/ 
void loadImage(const char* filenameIn, unsigned char** pimage, unsigned int* width, unsigned int* height)
{
	LodePNGColorType colorType = LCT_GREY;
	unsigned int bitdepth = 8;

	//decode	
	unsigned int error = lodepng_decode_file(pimage, width, height, filenameIn, colorType, bitdepth);

	//if there's an error, display it
	if(error)
	{
		printf("error %u: %s\n", error, lodepng_error_text(error));
	}

	//the pixels are now in the array "image", 1 byte per pixel
}

/**
Write a black&white PNG image to the hard drive. 

Input parameters:
	filenameOut - String containing the path where the file will be saved on the hard drive.
	image - Array containing the image contents. This is the data written to the disk.
	width - Width of the image.
	height - Height of the image.
*/ 
void writeImage(const char* filenameOut, const unsigned char* image, unsigned int width, unsigned int height)
{
	LodePNGColorType colorType = LCT_GREY;
	unsigned bitdepth = 8;
	
	//Encode the image
	unsigned error = lodepng_encode_file(filenameOut, image, width, height, colorType, bitdepth);

	//if there's an error, display it
	if(error)
	{
		printf("error %u: %s\n", error, lodepng_error_text(error));
	}
}
