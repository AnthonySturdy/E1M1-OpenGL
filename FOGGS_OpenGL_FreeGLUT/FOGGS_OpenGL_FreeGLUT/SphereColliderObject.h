#pragma once

#include "MeshObject.h"

class SphereColliderObject : public MeshObject {
public:
	SphereColliderObject(TexturedMesh* _mesh, Vector3 pos, float _radius);
	~SphereColliderObject();

	bool IsPointInCollider(Vector3 point);
	void Destroy();

	void Update();

private:
	float radius;
};