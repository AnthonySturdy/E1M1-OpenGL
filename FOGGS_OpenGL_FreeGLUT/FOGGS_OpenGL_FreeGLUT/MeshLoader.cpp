#include "MeshLoader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

namespace MeshLoader
{
	void LoadVertices(ifstream& inFile, TexturedMesh& mesh);
	void LoadTexCoords(ifstream& inFile, TexturedMesh& mesh);
	void LoadNormals(ifstream& inFile, TexturedMesh& mesh);
	void LoadIndices(ifstream& inFile, TexturedMesh& mesh);

	void LoadVertices(ifstream& inFile, TexturedMesh& mesh)
	{
		inFile >> mesh.vertexCount;

		if (mesh.vertexCount > 0)
		{
			mesh.vertices = new Vertex[mesh.vertexCount];

			for (int i = 0; i < mesh.vertexCount; i++)
			{
				inFile >> mesh.vertices[i].x >> mesh.vertices[i].y >> mesh.vertices[i].z;
			}
		}
	}

	void LoadTexCoords(ifstream& inFile, TexturedMesh& mesh) {
		inFile >> mesh.texCoordCount;

		if (mesh.texCoordCount > 0) {
			mesh.texCoords = new TexCoord[mesh.texCoordCount];

			for (int i = 0; i < mesh.texCoordCount; i++) {
				inFile >> mesh.texCoords[i].u >> mesh.texCoords[i].v;
			}
		}
	}

	void LoadNormals(ifstream& inFile, TexturedMesh& mesh) {
		inFile >> mesh.normalCount;

		if (mesh.normalCount > 0) {
			mesh.normals = new Vector3[mesh.normalCount];

			for (int i = 0; i < mesh.normalCount; i++) {
				inFile >> mesh.normals[i].x >> mesh.normals[i].y >> mesh.normals[i].z;
			}
		}
	}

	void LoadIndices(ifstream& inFile, TexturedMesh& mesh)
	{
		inFile >> mesh.indexCount;

		if (mesh.indexCount > 0) {
			mesh.vertexIndices = new GLushort[mesh.indexCount];

			for (int i = 0; i < mesh.indexCount; i++) {
				inFile >> mesh.vertexIndices[i];
			}
		}
	}

	/*Mesh* MeshLoader::Load(const char* path)
	{
		Mesh* mesh = new Mesh();

		ifstream inFile;
		inFile.open(path);
		if (!inFile.good())  
		{
			cerr  << "Can't open mesh file " << path << endl;
			return nullptr;
		}

		LoadVertices(inFile, *mesh);
		LoadNormals(inFile, *mesh);
		LoadIndices(inFile, *mesh);

		return mesh;
	}*/

	TexturedMesh* MeshLoader::LoadTextured(const char* path, Texture2D* texture) {
		TexturedMesh* tempMesh = new TexturedMesh();	//Create textured mesh

		//Load mesh file
		ifstream inFile;
		inFile.open(path);
		if (!inFile.good()) {
			cerr << "Can't open textured mesh file " << path << endl;
			return nullptr;
		}

		//Load TexturedMesh's Mesh variables, and the TexCoords in TexturedMesh
		LoadVertices(inFile, *tempMesh);
		LoadTexCoords(inFile, *tempMesh);
		LoadNormals(inFile, *tempMesh);
		LoadIndices(inFile, *tempMesh);

		tempMesh->texture = texture;

		return tempMesh;
	}

	TexturedMesh* MeshLoader::LoadOBJ(const char* path, Texture2D* texture) {
		std::ifstream inFile(path);
		std::string line;

		if (!inFile.is_open()) {
			std::cout << "Couldn't open file" << std::endl;
			return nullptr;
		}

		std::vector<GLushort*> vertexIndices, uvIndices, normalIndices;
		std::vector<Vertex*> tempVertices;
		std::vector<Vector3*> tempNormals;
		std::vector<TexCoord*> tempTexCoords;

		//Fill vectors with data, loop through file line by line and process each line
		while (std::getline(inFile, line)) {
			//Create stringstream of current line
			std::stringstream curLine(line);
			//Current type of line it's on (represented by first characters of line)
			std::string curDataType;
			curLine >> curDataType;

			if (curDataType == "v") {
				//Vertex
				Vertex* vert = new Vertex();
				curLine >> vert->x >> vert->y >> vert->z;
				tempVertices.push_back(vert);
			} else if (curDataType == "vn") {
				//Normal
				Vector3* norm = new Vector3();
				curLine >> norm->x >> norm->y >> norm->z;
				tempNormals.push_back(norm);
			} else if (curDataType == "vt") {
				//TexCoord
				TexCoord* uv = new TexCoord();
				curLine >> uv->u >> uv->v;
				tempTexCoords.push_back(uv);
			} else if (curDataType == "f") {
				//Face
				unsigned int vertIndex[3], uvIndex[3], normIndex[3];

				//For loop through all 3 'sections' of data on each line
				for (int i = 0; i < 3; i++) {
					std::string data;
					curLine >> data;

					std::stringstream intStream[3];
					int intStreamIndex = 0;

					for (int i = 0; i < data.length(); i++) {
						if (data[i] == '/') {
							intStreamIndex++;
						} else {
							intStream[intStreamIndex] << data[i];
						}
					}

					intStream[0] >> vertIndex[i];
					intStream[1] >> uvIndex[i];
					intStream[2] >> normIndex[i];
				}

				vertexIndices.push_back(new GLushort(vertIndex[0]));
				vertexIndices.push_back(new GLushort(vertIndex[1]));
				vertexIndices.push_back(new GLushort(vertIndex[2]));
				uvIndices.push_back(new GLushort(uvIndex[0]));
				uvIndices.push_back(new GLushort(uvIndex[1]));
				uvIndices.push_back(new GLushort(uvIndex[2]));
				normalIndices.push_back(new GLushort(normIndex[0]));
				normalIndices.push_back(new GLushort(normIndex[1]));
				normalIndices.push_back(new GLushort(normIndex[2]));
			}
		}

		//Convert vectors into object pointer arrays
		Vertex* v = new Vertex[tempVertices.size()];
		for (int i = 0; i < tempVertices.size(); i++) {
			v[i] = *tempVertices[i];
		}

		TexCoord* t = new TexCoord[tempTexCoords.size()];
		for (int i = 0; i < tempTexCoords.size(); i++) {
			t[i] = *tempTexCoords[i];
		}

		Vector3* n = new Vector3[tempNormals.size()];
		for (int i = 0; i < tempNormals.size(); i++) {
			n[i] = *tempNormals[i];
		}

		GLushort* vi = new GLushort[vertexIndices.size()];
		GLushort* ti = new GLushort[uvIndices.size()];
		GLushort* ni = new GLushort[normalIndices.size()];
		for (int i = 0; i < vertexIndices.size(); i++) {	//Indices should all be same size so we can use just 1 for loop
			vi[i] = *vertexIndices[i];
			ti[i] = *uvIndices[i];
			ni[i] = *normalIndices[i];
		}

		TexturedMesh* returnMesh = new TexturedMesh();

		returnMesh->vertices = v;
		returnMesh->texCoords = t;
		returnMesh->normals = n;

		returnMesh->vertexIndices = vi;
		returnMesh->uvIndices = ti;
		returnMesh->normalIndices = ni;

		returnMesh->vertexCount = tempVertices.size();
		returnMesh->texCoordCount = tempTexCoords.size();
		returnMesh->normalCount = tempNormals.size();

		returnMesh->indexCount = vertexIndices.size();	//This could be any of the indices, they should all be the same size.

		returnMesh->texture = texture;


		return returnMesh;
	}
}