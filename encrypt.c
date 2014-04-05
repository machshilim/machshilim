#include "lodepng.h"
#include "io_images.h"
#include <stdlib.h>
#include <malloc.h>
#include <time.h>  
#include <math.h>  

void shrinkImage(const unsigned char* inImage, unsigned char* outImage, unsigned int width, unsigned int height);
void binaryImage(unsigned char* image, unsigned int width, unsigned int height);
void initRandImg(unsigned char* randImage, unsigned int width, unsigned int height);
void encrypt(const unsigned char* image, const unsigned char* randIm1, unsigned char* randIm2,
			 unsigned int width, unsigned int height);

int main(int argc, char* argv[])
{
	// Variable declarations
	const char* filenameIn = (argc == 4) ? argv[1] : "YinYang.png";
	const char* filenameOut1 = (argc == 4) ? argv[2] : "first.png";
	const char* filenameOut2 = (argc == 4) ? argv[3] : "second.png";
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned char* image = NULL;
	unsigned char* shrunkImage = NULL;
	unsigned char* rand1 = NULL;
	unsigned char* rand2 = NULL;

	// Init srand for rand operations using the OS clock
	srand(time(NULL));

	// Load image to be encrypted from the hard drive
	loadImage(filenameIn, &image, &width, &height);
	
	shrunkImage = (unsigned char*) malloc( (width / 2) * height );
	rand1 = (unsigned char*) malloc( width * height );
	rand2 = (unsigned char*) malloc( width * height );

	if(!(shrunkImage == NULL || rand1 == NULL || rand2 == NULL))
	{
		shrinkImage(image, shrunkImage, width, height);
		binaryImage(shrunkImage, width / 2, height);
		initRandImg(rand1, width, height);
		initRandImg(rand2, width, height);
		encrypt(shrunkImage, rand1, rand2, width / 2, height);
		writeImage(filenameOut1, rand1, width, height);
		writeImage(filenameOut2, rand2, width, height);	
	}
	
	free(image);
	free(shrunkImage);
	free(rand1);
	free(rand2);
	return (0);
}

void shrinkImage(const unsigned char* inImage, unsigned char* outImage, unsigned int width, unsigned int height)
{
	unsigned int col = 0, row = 0;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col < width / 2 - 1; col++)
		{
			outImage[row * (width / 2) + col] = (char)  0.25 * inImage[row * width + 2 * col] +
														0.5  * inImage[row * width + 2 * col + 1] +
														0.25 * inImage[row * width + 2 * col + 2];
		}
		outImage[row * (width / 2) + col] = inImage[row * width + 2 * col + 1];
	}
}

void binaryImage(unsigned char* image, unsigned int width, unsigned int height)
{
	unsigned int col, row;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col < width; col++)
		{
			image[row * width + col] = (image[row * width + col] > 128) * 255;
		}
	}
}

void initRandImg(unsigned char* randImage, unsigned int width, unsigned int height)
{
	unsigned int col, row;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col < width; col += 2)
		{
			randImage[row * width + col] = (unsigned char) (( (double)rand() / (double)RAND_MAX ) > 0.5) * 255;
			randImage[row * width + col + 1] = 255 - randImage[row * width + col];
		}
	}
}

void encrypt(const unsigned char* image, const unsigned char* randIm1, unsigned char* randIm2,
			 unsigned int width, unsigned int height)
{
	unsigned int col, row;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col < width; col++)
		{
			if(image[row * width + col] == 255)
			{
				randIm2[row * 2 * width + 2 * col] = image[row * width + col] ^ randIm1[row * 2 * width + 2 * col];
				randIm2[row * 2 * width + 2 * col + 1] = image[row * width + col] ^ randIm1[row * 2 * width + 2 * col + 1];
			}
		}
	}
}
