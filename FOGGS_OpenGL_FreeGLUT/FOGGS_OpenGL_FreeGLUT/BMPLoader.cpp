#include "BMPLoader.h"

namespace BMPLoader {
	// Two characters to identify the bitmap file type
	struct bmpfile_ft {
		char magic[2];
	};

	// Bitmap header file
	struct bmpfile_header {
		int filesz;
		int creator1;
		int creator2;
		int bmp_offset;
	};

	// Bitmap info
	struct BITMAPINFOHEADER {
		//int header_sz;
		int width;
		int height;
		int nplanes;
		int bitspp;
		int compress_type;
		int bmp_bytesz;
		int hres;
		int vres;
		int ncolors;
		int nimpcolors;
	};

	struct bmp_color {
		unsigned char b, g, r;
	};

	BITMAPINFOHEADER bitMapInfo;

	void BMPLoader::LoadBitMap(const char* fileName, const char* outFileName) {
		bmpfile_ft magic;
		bmpfile_header header;
		int row, col;
		bmp_color* image_buffer;

		FILE* inFile;
		FILE* outFile;
		inFile = fopen(fileName, "rb");
		outFile = fopen(outFileName, "wb");
		if (inFile == NULL) {
			std::cout << "Can't open file " << fileName << std::endl;
			return;
		}
		std::cout << "Opened BMP file " << fileName << std::endl;
		fread(&magic, sizeof(bmpfile_ft), 1, inFile);
		fread(&header, sizeof(bmpfile_header), 1, inFile);
		fread(&bitMapInfo, sizeof(BITMAPINFOHEADER), 1, inFile);
		if (bitMapInfo.width != bitMapInfo.height) {
			std::cout << "Bitmap is not square! " << fileName << std::endl;
			return;
		}

		// Create a buffer to hold each line as it is read in
		image_buffer = (bmp_color*)malloc(sizeof(bmp_color) * bitMapInfo.width);

		for (row = 0; row < bitMapInfo.height; row++) {
			fread(image_buffer, sizeof(bmp_color), bitMapInfo.width, inFile);
			// bmp file store colours in the order blue, green, red
			// we need to rearrange to the order red, green, blue
			for (col = 0; col < bitMapInfo.width; col++) {
				putc(image_buffer[col].r, outFile);
				putc(image_buffer[+col].g, outFile);
				putc(image_buffer[col].b, outFile);
			}
		}

		// close the .bmp and free up memory
		fclose(inFile);
		fclose(outFile);
		free(image_buffer);
	}
}