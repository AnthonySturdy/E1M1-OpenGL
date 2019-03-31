#include "SceneObject.h"

SceneObject::SceneObject(TexturedMesh* _mesh) {
	mesh = _mesh;
}

SceneObject::~SceneObject(void) {
	delete mesh;
}