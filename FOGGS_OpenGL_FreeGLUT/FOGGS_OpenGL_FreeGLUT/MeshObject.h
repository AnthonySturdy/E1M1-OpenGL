#pragma once
#include<Windows.h>		//Required for OpenGL on Windows
#include<gl/GL.h>		//OpenGL
#include<gl/GLU.h>		//OpenGL Utilities
#include"GL/freeglut.h"	//FreeGLUT library

#include"Structures.h"
#include"SceneObject.h"
#include"Texture2D.h"

class MeshObject : public SceneObject {
private:
	float rotation;
	Vector3 position;
	Material* material;

public:
	MeshObject(TexturedMesh* _mesh, Vector3 pos);
	~MeshObject(void);

	void Draw();
	void Update();

	void DrawIndexedCube();
};