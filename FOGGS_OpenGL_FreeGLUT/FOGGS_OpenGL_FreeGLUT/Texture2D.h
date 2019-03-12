#pragma once
#include<fstream>
#include<iostream>
#include<Windows.h>		//Required for OpenGL on Windows
#include<gl/GL.h>		//OpenGL
#include<gl/GLU.h>		//OpenGL Utilities
#include"GL/freeglut.h"	//FreeGLUT library

class Texture2D {
private:
	GLuint _ID;	//Texture ID
	int _width, _height;

public:
	Texture2D();
	~Texture2D();

	bool LoadFromRAW(const char* path, int w, int h);
	bool LoadFromData(char* data, int w, int h);

	GLuint GetID() const { return _ID; }
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
};