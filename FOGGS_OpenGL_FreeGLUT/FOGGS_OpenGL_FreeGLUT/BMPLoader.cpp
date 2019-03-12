#include "BMPLoader.h"

namespace BMPLoader {
	struct BitMap {
		unsigned int width, height;
		unsigned char* pixels;
	};

	BitMap* bmp;

	char* BMPLoader::LoadBitMap(const char* fileName) {
		//Reset bmp from previous texture
		if (bmp && bmp->pixels) {
			bmp->width = 0;
			bmp->height = 0;

			delete bmp->pixels;
			delete bmp;

			bmp = nullptr;
		} else {
			bmp = nullptr;
		}

		std::ifstream inFile(fileName, std::ifstream::binary);
		if (inFile.good()) {
			bmp = new BitMap();
			bmp->width = 0;
			bmp->height = 0;
			bmp->pixels = nullptr;

			unsigned char info[54] = { 0 };
			inFile.read((char*)&info[0], 54);

			//Ask about "*(unsigned int*)&", without the *'s and & symbol it returns 0.
			bmp->width = *(unsigned int*)&info[18];
			bmp->height = *(unsigned int*)&info[22];

			unsigned int size = ((((bmp->width * bmp->height) + 31) & ~31) / 8) * bmp->height;
			bmp->pixels = new unsigned char[size];
			inFile.read((char*)bmp->pixels, size);
			inFile.close();

			//Change BGR to RGB
			for (int i = 0; i < size / 3; i++) {
				unsigned char temp = bmp->pixels[i];
				bmp->pixels[i] = bmp->pixels[i + 2];
				bmp->pixels[1 + 2] = temp;
			}

			return (char*)bmp->pixels;
		}

		return nullptr;
	}
}