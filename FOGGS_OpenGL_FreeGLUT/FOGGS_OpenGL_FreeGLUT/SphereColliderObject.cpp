#include "SphereColliderObject.h"

SphereColliderObject::SphereColliderObject(TexturedMesh* _mesh, Vector3 pos, float _radius) : MeshObject(_mesh, pos) {
	radius = _radius;
}

SphereColliderObject::~SphereColliderObject() {

}

bool SphereColliderObject::IsPointInCollider(Vector3 point) {

	return false;
}

void SphereColliderObject::Destroy() {

}

void SphereColliderObject::Update() {

}
