#include "SceneGraph.h"

#include<iostream>

namespace SceneGraph {
	void SceneGraph::Traverse(BinaryNode* n) {
		static int numTabs = 0;
		numTabs++;
		std::cout << "\n";
		for (int i = 0; i < numTabs; i++) {
			std::cout << "\t";

			if (n->child != nullptr) {
				std::cout << n->label.c_str();

				Traverse(n->child);
			}

			numTabs--;
			if (n->sibling != nullptr) {
				Traverse(n->sibling);
			}
		}
	}

	BinaryNode* SceneGraph::MakeNode(SceneObject* data, std::string label) {
		BinaryNode* newNode;

		newNode = new BinaryNode();
		newNode->data = data;
		newNode->label = label;
		newNode->child = nullptr;
		newNode->sibling = nullptr;

		return newNode;
	}
}