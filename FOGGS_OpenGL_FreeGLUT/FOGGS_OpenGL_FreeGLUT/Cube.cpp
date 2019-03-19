#include "Cube.h"
#include<math.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>

Cube::Cube(TexturedMesh* _mesh, Vector3 pos) : SceneObject(_mesh) {
	rotation = 0.0f;
	position = pos;
	material = new Material(Vector4(1.0f, 1.0f, 1.0f, 1.0f),
							Vector4(1.0f, 1.0f, 1.0f, 1.0f),
							Vector4(1.0f, 1.0f, 1.0f, 1.0f),
							100.0f);
}

Cube::~Cube() {

}

void Cube::Draw() {
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glRotatef(rotation, 1.0f, 1.0f, 0.0f);
	DrawIndexedCube();
	glPopMatrix();
}

void Cube::Update() {
	rotation += 0.5f;
}

void Cube::DrawIndexedCube() {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh->indexCount; i++) {
		std::cout << "index: " << i << std::endl;
		//GETTING ERROR HERE AFTER A FEW ITERATIONS
		glVertex3fv(&mesh->vertices[mesh->vertexIndices[i]].x);
		glTexCoord2fv(&mesh->texCoords[mesh->uvIndices[i]].u);
		glNormal3fv(&mesh->normals[mesh->normalIndices[i]].x);
	}
	glEnd();
}