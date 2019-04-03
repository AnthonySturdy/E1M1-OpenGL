#pragma once
#include<Windows.h>		//Required for OpenGL on Windows
#include<gl/GL.h>		//OpenGL
#include<gl/GLU.h>		//OpenGL Utilities
#include"GL/freeglut.h"	//FreeGLUT library

#include"Structures.h"
#include"SceneObject.h"
#include"Texture2D.h"

class MeshObject : public SceneObject {
protected:
	float rotation;
	Vector3 position;
	Material* material;

public:
	MeshObject(TexturedMesh* _mesh, Vector3 pos);
	~MeshObject(void);

	virtual void Draw();
	virtual void Update(float deltaTime);

	void DrawIndexedCube();


	virtual bool IsPointInCollider(Vector3 point) { std::cout << "IsPointInCollider() function hasn't been overridden! Is being called from MeshObject." << std::endl;  return false; };

};