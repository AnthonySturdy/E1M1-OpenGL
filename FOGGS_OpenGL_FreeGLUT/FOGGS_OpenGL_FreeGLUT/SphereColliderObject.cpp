#include "SphereColliderObject.h"

SphereColliderObject::SphereColliderObject(TexturedMesh* _mesh, Vector3 pos, float _radius) : MeshObject(_mesh, pos) {
	radius = _radius;
}

SphereColliderObject::~SphereColliderObject() {

}

bool SphereColliderObject::IsPointInCollider(Vector3 point) {
	float dist = sqrt(pow(point.x - position.x, 2) + pow(point.y - position.y, 2) + pow(point.z - position.z, 2));

	if (dist <= radius) {
		return true;
	}

	return false;
}

void SphereColliderObject::Update(float deltaTime) {
	rotation += deltaTime * 40;
}
