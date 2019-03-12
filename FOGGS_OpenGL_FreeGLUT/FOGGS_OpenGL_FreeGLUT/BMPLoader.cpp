#include "BMPLoader.h"

namespace BMPLoader {
	struct bitmap {
		unsigned int width, height;
		unsigned char* pixels;
	};

	bitmap* bmp;

	char* BMPLoader::LoadBitMap(const char* fileName) {
		if (bmp && bmp->pixels) {
			bmp->width = 0;
			bmp->height = 0;
			free(bmp->pixels);
			free(bmp);
			bmp = NULL;
		} else {
			bmp = NULL;
		}

		FILE* f = fopen(fileName, "rb");

		if (f) {
			bmp = (bitmap*)malloc(sizeof(bitmap));
			bmp->width = 0;
			bmp->height = 0;
			bmp->pixels = NULL;

			unsigned char info[54] = { 0 };
			fread(info, sizeof(unsigned char), 54, f);

			bmp->width = *(unsigned int*)&info[18];
			bmp->height = *(unsigned int*)&info[22];

			unsigned int size = ((((bmp->width * bmp->height) + 31) & ~31) / 8) * bmp->height;
			bmp->pixels = (unsigned char*)malloc(size);
			fread(bmp->pixels, sizeof(unsigned char), size, f);
			fclose(f);

			//Changes RGB to BGR
			for (int i = 0; i < size; i += 3) {
				unsigned char tmp = bmp->pixels[i];
				bmp->pixels[i] = bmp->pixels[i + 2];
				bmp->pixels[i + 2] = tmp;
			}

			return (char*)bmp->pixels;
		}

		return nullptr;
	}
}