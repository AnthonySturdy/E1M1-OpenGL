#pragma once

#include"Structures.h"

class SceneObject {
protected:
	TexturedMesh* mesh;

	bool isActive = true;

public:
	SceneObject(TexturedMesh* _mesh);
	virtual ~SceneObject(void);

	virtual void Draw() = 0;
	virtual void Update(float deltaTime) = 0;

	virtual bool IsPointInCollider(Vector3 point) = 0;

	bool GetIsActive() { return isActive; }
	void SetIsActive(bool active) { isActive = active; }
};