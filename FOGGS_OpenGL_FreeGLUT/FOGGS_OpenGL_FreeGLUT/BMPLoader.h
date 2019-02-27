#pragma once
#pragma warning(disable:4996)
#include<fstream>
#include<iostream>
#include<cstdio>

// BMP Reader Converted from C to C++

//TODO: Ask teacher about this, and fixing it.

namespace BMPLoader {
	void LoadBitMap(const char* fileName, const char* outFileName);
}