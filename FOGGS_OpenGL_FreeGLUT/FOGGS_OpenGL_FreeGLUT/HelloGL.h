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
#include"Texture2D.h"
#include"BMPLoader.h"
#include"LinkedList.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define REFRESHRATE 16

#define MOUSE_SENSITIVITY 0.1
#define PLAYER_WALK_SPEED 0.2
#define PLAYER_HEIGHT 8


class HelloGL {
public:
	HelloGL(int argc, char* argv[]);
	~HelloGL();

	void InitObjects();
	void InitGL(int argc, char* argv[]);

	void Display();
	void Update();
	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);

private:
	Camera* camera;
	float xAngle = 0;
	float yAngle = 0;

	std::vector<SceneObject*> objects;

	void Movement();
	bool isMovingForward, isMovingBackward, isMovingLeft, isMovingRight;

	Vector4* lightPosition;
	Lighting* lightData;
};