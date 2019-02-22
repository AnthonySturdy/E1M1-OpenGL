#include "Cube.h"
#include<math.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>

Cube::Cube(Mesh* _mesh, Vector3 pos) : SceneObject(_mesh) {
	rotation = 0.0f;
	position = pos;
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
	rotation += 3.0f;
}

void Cube::DrawIndexedCube() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mesh->vertices);
	glColorPointer(3, GL_FLOAT, 0, mesh->colors);

	glPushMatrix();
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_SHORT, mesh->indices);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}