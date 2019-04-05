#pragma once

#include"GL/freeglut.h"
#include"Texture2D.h"
//#include"SceneObject.h"	//THIS IS WHERE ERROR IS
class SceneObject;
#include<string>

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

struct Vector4 {
	float x, y, z, w;

	Vector4() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
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

struct TexCoord {
	float u, v;

	TexCoord() {
		u = 0;
		v = 0;
	}

	TexCoord(float _u, float _v) {
		u = _u;
		v = _v;
	}
};

struct Triangle {
	Vertex* v1;
	Vertex* v2;
	Vertex* v3;

	Triangle() {

	}

	Triangle(Vertex* _v1, Vertex* _v2, Vertex* _v3) {
		v1 = _v1;
		v2 = _v2;
		v3 = _v3;
	}
};

struct Mesh {
	Vertex* vertices;
	Vector3* normals;

	GLushort* vertexIndices;
	GLushort* uvIndices;
	GLushort* normalIndices;

	int vertexCount, normalCount, indexCount;
};

struct TexturedMesh {
	Texture2D* texture;

	Vertex* vertices;
	Vector3* normals;
	TexCoord* texCoords;

	GLushort* vertexIndices;
	GLushort* uvIndices;
	GLushort* normalIndices;

	Triangle* tris;	//Not used for rendering

	int vertexCount, normalCount, texCoordCount, indexCount;
};

struct Lighting {
	Vector4 ambient, diffuse, specular;

	Lighting(Vector4 _ambient, Vector4 _diffuse, Vector4 _specular) {
		ambient = _ambient;
		diffuse = _diffuse;
		specular = _specular;
	}
};

struct Material {
	Vector4 ambient, diffuse, specular;
	GLfloat shininess;

	Material(Vector4 _ambient, Vector4 _diffuse, Vector4 _specular, GLfloat _shininess) {
		ambient = _ambient;
		diffuse = _diffuse;
		specular = _specular;
		shininess = _shininess;
	}
};

struct ListNode {
	int* data;
	ListNode* next = NULL;

	ListNode(int* _data) {
		data = _data;
	}
};

struct BinaryNode {
	std::string label;
	SceneObject* data;

	BinaryNode* child;
	BinaryNode* sibling;
};