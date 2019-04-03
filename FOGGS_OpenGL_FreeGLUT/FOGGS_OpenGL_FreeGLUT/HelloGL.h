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
#include"SphereColliderObject.h"
#include"Texture2D.h"
#include"BMPLoader.h"
#include"LinkedList.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define REFRESHRATE 16

#define MOUSE_SENSITIVITY 3
#define PLAYER_WALK_SPEED 16
#define PLAYER_SPRINT_MULTIPLIER 1.8
#define PLAYER_HEIGHT 4.5
#define PLAYER_MAX_STEP_HEIGHT 2.5
#define PLAYER_STEP_UP_SPEED 18
#define PLAYER_STEP_DOWN_SPEED 22


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
	void SpecialDown(int key, int x, int y);
	void SpecialUp(int key, int x, int y);

private:
	float prevElapsedTime = 0;
	float deltaTime;

	Camera* camera;
	float xAngle = 0;
	float yAngle = 0;
	void CameraLook();

	void Movement();
	bool isMovingForward, isMovingBackward, isMovingLeft, isMovingRight;
	bool isSprinting;
	bool CanPlayerMove();
	float prevGroundHeight = 0;

	std::vector<SceneObject*> objects;
	TexturedMesh* navigationMesh;
	SceneObject* armourObject1;
	SceneObject* armourObject2;
	SceneObject* gunObject;
	void DebugNavigationMesh();

	float GetTriangleHeight(Triangle tri);	//Only used for navigation mesh!
	bool IsPointInsideTriangle(Vector3 s, Triangle tri); //This is 2D not 3D
	float GetGroundHeightAtPoint(Vector3 point, TexturedMesh* mesh);

	Vector4* lightPosition;
	Lighting* lightData;
};