#include <stdio.h>
#include <stdlib.h>

#include "libbmp.h"

void displayImage(char input[])
{
	bmp_img img;
	bmp_img_init_df(&img, 0, 0);

	bmp_img_read(&img, input);

	int width = img.img_header.biWidth;
	int height = img.img_header.biHeight;

	for(int i = 0; i < height; i += 2)
	{
		for(int j = 0; j < width; j++)
		{
			bmp_pixel pixel1 = img.img_pixels[j][i];
			bmp_pixel pixel2 = img.img_pixels[j][i + 1];
			//background => upper part
			printf("\x1b[48;2;%d;%d;%dm", pixel1.red, pixel1.green, pixel1.blue);
			// foreground => lower part
			printf("\x1b[38;2;%d;%d;%dm", pixel2.red, pixel2.green, pixel2.blue);
			
			printf("▄");

			printf("\x1b[0m");

		}

		printf("\n");

	}

}

int main(int argc, char *argv[])
{
	char input[100];
	sprintf(input, "%s", argv[1]);
	displayImage(input);
	return(0);

}