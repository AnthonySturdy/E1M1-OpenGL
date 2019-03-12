#include "Cube.h"
#include<math.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>

Cube::Cube(TexturedMesh* _mesh, Vector3 pos) : SceneObject(_mesh) {
	rotation = 0.0f;
	position = pos;
	material = new Material(Vector4(0.8f, 0.05f, 0.05f, 1.0f),
							Vector4(0.8f, 0.05f, 0.05f, 1.0f),
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
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, mesh->mesh->vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, mesh->texCoords);
	glNormalPointer(GL_FLOAT, 0, mesh->mesh->normals);

	glMaterialfv(GL_FRONT, GL_AMBIENT, &(material->ambient.x));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &(material->diffuse.x));
	glMaterialfv(GL_FRONT, GL_SPECULAR, &(material->specular.x));
	glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);

	glBindTexture(GL_TEXTURE_2D, mesh->texture->GetID());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPushMatrix();
	glDrawElements(GL_TRIANGLES, mesh->mesh->indexCount, GL_UNSIGNED_SHORT, mesh->mesh->indices);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}