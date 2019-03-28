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
#include"MeshObject.h"
#include"Texture2D.h"
#include"BMPLoader.h"
#include"LinkedList.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define REFRESHRATE 16

#define MOUSE_SENSITIVITY 0.1
#define PLAYER_WALK_SPEED 0.3
#define PLAYER_HEIGHT 4.5


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
	void CameraLook();

	std::vector<SceneObject*> objects;
	TexturedMesh* navigationMesh;
	void DebugNavigationMesh();

	void Movement();
	bool isMovingForward, isMovingBackward, isMovingLeft, isMovingRight;

	float GetTriangleHeight(Triangle tri);	//Only used for navigation mesh!
	bool IsPointInsideTriangle(Vector3 s, Triangle tri); //This is 2D not 3D
	float GetGroundHeightAtPoint(Vector3 point, TexturedMesh* mesh);

	Vector4* lightPosition;
	Lighting* lightData;
};