#include "SceneGraph.h"

#include<iostream>

namespace SceneGraph {
	void SceneGraph::Traverse(BinaryNode* n) {
		static int numTabs = -1;
		numTabs++;
		std::cout << "\n";
		for (int i = 0; i < numTabs; i++) {
			std::cout << "|\t";
		}

		std::cout << n->label.c_str();

		if (n->child != nullptr) {
			Traverse(n->child);
		} 

		numTabs--;
		if (n->sibling != nullptr) {
			//std::cout << n->sibling->label.c_str();
			Traverse(n->sibling);
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

	void SceneGraph::DeleteTree(BinaryNode * n) {
		if (n->child != nullptr) {
			DeleteTree(n->child);
		}

		if (n->sibling != nullptr) {
			DeleteTree(n->sibling);
		}

		std::cout << "\n" << n->label << " freed.";
		delete n;
		n = nullptr;
	}
}