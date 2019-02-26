#pragma once

#include"Structures.h"

class SceneObject {
protected:
	TexturedMesh* mesh;

public:
	SceneObject(TexturedMesh* _mesh);
	virtual ~SceneObject(void);

	virtual void Draw() = 0;
	virtual void Update() = 0;
};