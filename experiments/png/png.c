#include <stdio.h>
#include <stdlib.h>

#include "lodepng.h"

typedef struct Pixel
{
	int r;
    int g;
    int b;
	int a;

}Pixel;

void displayImage(char input[])
{
	unsigned char* image = 0;
	unsigned width, height;

	lodepng_decode32_file(&image, &width, &height, input);

	for(int i = 0; i < height; i += 2)
	{
		for(int j = 0; j < width; j++)
		{
			Pixel pixel1;
			pixel1.r = image[i * width * 4 + j * 4];
			pixel1.g = image[i * width * 4 + j * 4 + 1];
			pixel1.b = image[i * width * 4 + j * 4 + 2];
			pixel1.a = image[i * width * 4 + j * 4 + 3];

			Pixel pixel2;
			pixel2.r = image[(i + 1) * width * 4 + j * 4];
			pixel2.g = image[(i + 1) * width * 4 + j * 4 + 1];
			pixel2.b = image[(i + 1) * width * 4 + j * 4 + 2];
			pixel2.a = image[(i + 1) * width * 4 + j * 4 + 3];

			printf("\x1b[48;2;%d;%d;%dm", pixel1.r, pixel1.g, pixel1.b);
			printf("\x1b[38;2;%d;%d;%dm", pixel2.r, pixel2.g, pixel2.b);

			printf("▄");

			printf("\x1b[0m");

		}

		printf("\n");
	}

	free(image);
}

int main(int argc, char *argv[])
{
	char input[100];
	sprintf(input, "%s", argv[1]);
	displayImage(input);
	return(0);

}
