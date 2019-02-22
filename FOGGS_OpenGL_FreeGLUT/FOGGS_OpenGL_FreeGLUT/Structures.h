#pragma once

#include"GL/freeglut.h"

struct Vector3 {
	float x, y, z;

	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
};

struct Camera {
	Vector3 eye, centre, up;
};

struct Color {
	GLfloat r, g, b;
};

struct Vertex {
	GLfloat x, y, z;
};

struct Mesh {
	Vertex* vertices;
	Color* colors;
	GLushort* indices;
	int vertexCount, colorCount, indexCount;
};