#pragma once
#include "MeshObject.h"

class BoxCollision : public MeshObject {
public:
	BoxCollision(TexturedMesh* _mesh, Vector3 pos);
	~BoxCollision();

	void Update(float deltaTime);

private:
	Box box;
};