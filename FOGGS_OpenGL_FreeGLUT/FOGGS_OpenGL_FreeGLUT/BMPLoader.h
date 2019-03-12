#pragma once
#pragma warning(disable:4996)
#include<fstream>
#include<iostream>
#include<cstdio>

// 24 bit BMP reader. Code from: https://www.daniweb.com/programming/software-development/threads/482083/reading-a-24-bit-bmp-image-file

namespace BMPLoader {
	char* LoadBitMap(const char* fileName);
}