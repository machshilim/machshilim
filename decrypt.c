#include "lodepng.h"
#include "io_images.h"
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

void flipImageHorizontal(unsigned char* image, unsigned int width, unsigned int height);

void decrypt(
    unsigned char* image,
    const unsigned char* key,
    const unsigned char* e_output,
    unsigned int width,
    unsigned int height
);

int main(int argc, char* argv[]) {
	const char* output = (argc >= 2) ? argv[1] : "output.png";
	const char* filename_key = (argc >= 3) ? argv[2] : "first.png"; // first.png
	const char* filename_encrypted = (argc >= 4) ? argv[3] : "second.png"; // second.png
	unsigned int width = 0;
	unsigned int height = 0;

	unsigned char* shrunk_img = NULL;
	unsigned char* key_img = NULL;
	unsigned char* encrypted_img = NULL;

	loadImage(filename_key, &key_img, &width, &height);
	loadImage(filename_encrypted, &encrypted_img, &width, &height);

	shrunk_img = (unsigned char*)malloc((width / 2) * height);

    flipImageHorizontal(encrypted_img, width, height);

    decrypt(shrunk_img, key_img, encrypted_img, width / 2, height);

    writeImage(output, shrunk_img, width / 2, height);

	free(shrunk_img);
	free(key_img);
	free(encrypted_img);

    return 0;
}

void decrypt(
    unsigned char* image,
    const unsigned char* key,
    const unsigned char* e_output,
    unsigned int width,
    unsigned int height
) {
	unsigned int col, row;
	for (row = 0; row < height; row++)
	{
		for (col = 0; col < width; col++)
		{
            unsigned const key1 = key[row * 2 * width + 2 * col];
            unsigned const key2 = key[row * 2 * width + 2 * col + 1];
            unsigned const eo1 = e_output[row * 2 * width + 2 * col];
            unsigned const eo2 = e_output[row * 2 * width + 2 * col + 1];
            unsigned const xor0 = key1 ^ eo1;
            unsigned const xor1 = key2 ^ eo2;
            if (xor0 == 255 && xor1 == 255) {
                image[row * width + col] = 255;
            }
		}
	}
}

void flipImageHorizontal(unsigned char* image, unsigned int width, unsigned int height)
{
	unsigned int col, row;
	char temp;
	for (row = 0; row < height; row++)
	{
		for (col = 0; col <= width / 2; col++)
		{
			temp = image[row * width + col];
			image[row * width + col] = image[row * width + (width - 1) - col];
			image[row * width + (width - 1) - col] = temp;
		}
	}
}
