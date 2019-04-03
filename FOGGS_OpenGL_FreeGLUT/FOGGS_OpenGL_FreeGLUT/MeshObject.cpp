#include"MeshObject.h"
#include<math.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>

MeshObject::MeshObject(TexturedMesh* _mesh, Vector3 pos) : SceneObject(_mesh) {
	rotation = 0.0f;
	position = pos;
	material = new Material(Vector4(0.05f, 0.8f, 0.05f, 1.0f),
							Vector4(0.05f, 0.8f, 0.05f, 1.0f),
							Vector4(1.0f, 1.0f, 1.0f, 1.0f),
							100.0f);
}

MeshObject::~MeshObject() {
	delete material;
}

void MeshObject::Draw() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, &(material->ambient.x));
	glMaterialf(GL_FRONT, GL_SHININESS, material->shininess);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, mesh->texture->GetID());	//Bind the texture to the ID
	glTranslatef(position.x, position.y, position.z);
	glRotatef(rotation, 0.0f, 1.0f, 0.0f);
	DrawIndexedCube();
	glPopMatrix();
}

void MeshObject::Update(float deltaTime) {
	//rotation += 0.5f;
}

void MeshObject::DrawIndexedCube() {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh->indexCount; i++) {
		//The order of this matters!!!! Pass in vertices last or else it will be broken!!!
		glNormal3fv(&mesh->normals[mesh->normalIndices[i] - 1].x);
		glTexCoord2fv(&mesh->texCoords[mesh->uvIndices[i] - 1].u);
		glVertex3fv(&mesh->vertices[mesh->vertexIndices[i] - 1].x);
	}
	glEnd();
}