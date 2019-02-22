#pragma once

#include"Structures.h"

class SceneObject {
protected:
	Mesh* mesh;

public:
	SceneObject(Mesh* _mesh);
	virtual ~SceneObject(void);

	virtual void Draw() = 0;
	virtual void Update() = 0;
};