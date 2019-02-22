#pragma once

#include<Windows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<vector>
#include"GL/freeglut.h"

#include"GLUTCallbacks.h"
#include"Structures.h"
#include"SceneObject.h"
#include"MeshLoader.h"
#include"Cube.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define REFRESHRATE 16

class HelloGL {
public:
	HelloGL(int argc, char* argv[]);
	~HelloGL();

	void InitObjects();
	void InitGL(int argc, char* argv[]);

	void Display();
	void Update();
	void Keyboard(unsigned char key, int x, int y);

private:
	Camera* camera;

	std::vector<SceneObject*> objects;

	static Vertex vertices[];
	static Color colours[];

	static Vertex indexedVertices[];
	static Color indexedColors[];
	static GLushort indices[];
};