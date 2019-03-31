#include "BoxCollision.h"

BoxCollision::BoxCollision(TexturedMesh* _mesh, Vector3 pos) : MeshObject(_mesh, pos) {

}

BoxCollision::~BoxCollision() {

}

void BoxCollision::Update(float deltaTime) {
	rotation += deltaTime;
}