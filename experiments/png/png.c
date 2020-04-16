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

int main(void)
{
	unsigned char* image = 0;
	unsigned width, height;

	lodepng_decode32_file(&image, &width, &height, "test.png");

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			Pixel pixel;
			pixel.r = image[i * width * 4 + j * 4];
			pixel.g = image[i * width * 4 + j * 4 + 1];
			pixel.b = image[i * width * 4 + j * 4 + 2];
			pixel.a = image[i * width * 4 + j * 4 + 3];

			// foreground => lower part
			printf("\x1b[38;2;%d;%d;%dm", pixel.r, pixel.g, pixel.b);

			printf("██");

			printf("\x1b[0m");

		}

		printf("\n");
	}

	free(image);
}
