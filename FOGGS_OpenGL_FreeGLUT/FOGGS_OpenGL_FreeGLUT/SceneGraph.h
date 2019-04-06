#pragma once
#include "Structures.h"

namespace SceneGraph {
	void Traverse(BinaryNode* n);
	BinaryNode* MakeNode(SceneObject* data, std::string label);
	void DeleteTree(BinaryNode* n);
}