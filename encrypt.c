#include "lodepng.h"
#include "io_images.h"
#include <stdlib.h>
#include <malloc.h>
#include <time.h>  
#include <math.h>  

void shrinkImage(const unsigned char* inImage, unsigned char* outImage, unsigned int width, unsigned int height);
void binaryImage(unsigned char* image, unsigned int width, unsigned int height);
void initRandImage(unsigned char* randImage, unsigned int width, unsigned int height);
void flipImageHorizontal(unsigned char* image, unsigned int width, unsigned int height);
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
	
	// Note: all the images in the program are represented using
	// a 1D array and not 2D array as usual. We do it as follows -
	// first we save the first row of pixels in the array, after them
	// the second row, then the third, etc. See function below.
	unsigned char* image = NULL;
	unsigned char* shrunkImage = NULL;
	unsigned char* rand1 = NULL;
	unsigned char* rand2 = NULL;

	// Init srand for rand operations using the OS clock.
	srand(time(NULL));

	// Load image to be encrypted from the hard drive.
	loadImage(filenameIn, &image, &width, &height);
	
	// Allocate memory for the images used in the algorithm.	
	shrunkImage = (unsigned char*) malloc( (width / 2) * height );
	rand1 = (unsigned char*) malloc( width * height );
	rand2 = (unsigned char*) malloc( width * height );

	if(!(shrunkImage == NULL || rand1 == NULL || rand2 == NULL))
	{
		// Every pixel in the input image affects two pixels in 
		// the encrypted images so we have to shrink it by 2.
		shrinkImage(image, shrunkImage, width, height);
		
		// Turn a gray-scale image (which contain values between
		// 0-255) into a black & white image (which contains only
		// 0 and 255).
		binaryImage(shrunkImage, width / 2, height);
		
		// Initialize two random images.
		initRandImage(rand1, width, height);
		initRandImage(rand2, width, height);
		
		// Encrypt the input and save the encryted result to
		// rand1 and rand2.
		encrypt(shrunkImage, rand1, rand2, width / 2, height);
		
		// Flip the second image.
		flipImageHorizontal(rand2, width, height);
		
		// Save results to hard drive.
		writeImage(filenameOut1, rand1, width, height);
		writeImage(filenameOut2, rand2, width, height);	
	}
	
	free(image);
	free(shrunkImage);
	free(rand1);
	free(rand2);
	return (0);
}

/**
The function shrink the image in the horizontal direction by a factor 2.
For example if the input image had dimensions 200 (height) by 500 (width),
after shrinking we will get an image of 200 (height) by 250 (width).

Input:
	inImage - The input image in its original size.
	outImage - The array which will contain the shrunken image. Memory is assumed allocated.
	width - Width of the input image (the output image will be width / 2).
	height - Height of the input image.
*/
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

/**
Turn a gray-scale image (which contain values between 0-255) into a black
& white image (which contains only 0 [black pixel] and 255 [white pixel]).

Input:
	image - The gray scale image. At the end all its values will be 0 or 255.
	width - Width of the input image (the output image will be width / 2).
	height - Height of the input image.
*/
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

/**
Initialize a random image.
The pixels of each row in the image are paired. The left pixel of each pair
is random (0 for black, 255 for white). His right neighbour receives the other
color.

Input:
	randImage - Pointer to the memory block where the image is saved. Memory assumed allocated.
	width - Width of the random image. 
	height - Height of the random image.
*/
void initRandImage(unsigned char* randImage, unsigned int width, unsigned int height)
{
	unsigned int col, row;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col < width; col += 2)
		{
			// Left pixel is assigned a random color (0 = black, 255 = white)
			randImage[row * width + col] = (unsigned char) (( (double)rand() / (double)RAND_MAX ) > 0.5) * 255;
			// Right pixel receives the opposite color from the left one.
			randImage[row * width + col + 1] = 255 - randImage[row * width + col];
		}
	}
}

/**
Flips an image horizontally.

Input:
	image - The image that will be flipped.
	width - Width of the input image.
	height - Height of the input image.
*/
void flipImageHorizontal(unsigned char* image, unsigned int width, unsigned int height)
{
	unsigned int col, row;
	char temp;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col <= width / 2; col++)
		{
			temp = image[row * width + col];
			image[row * width + col] = image[row * width + (width - 1) - col];
			image[row * width + (width - 1) - col] = temp;
		}
	}
}

/**
The encryption. The white values of the input image determine the relation
of the random images. The black pixels of the input image make no effect. 
*/
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
