#include "MeshLoader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

namespace MeshLoader
{
	void LoadVertices(ifstream& inFile, Mesh& mesh);
	void LoadTexCoords(ifstream& inFile, TexturedMesh& mesh);
	void LoadNormals(ifstream& inFile, Mesh& mesh);
	void LoadIndices(ifstream& inFile, Mesh& mesh);

	void LoadVertices(ifstream& inFile, Mesh& mesh)
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

	void LoadNormals(ifstream& inFile, Mesh& mesh) {
		inFile >> mesh.normalCount;

		if (mesh.normalCount > 0) {
			mesh.normals = new Vector3[mesh.normalCount];

			for (int i = 0; i < mesh.normalCount; i++) {
				inFile >> mesh.normals[i].x >> mesh.normals[i].y >> mesh.normals[i].z;
			}
		}
	}

	void LoadIndices(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.indexCount;

		if (mesh.indexCount > 0) {
			mesh.indices = new GLushort[mesh.indexCount];

			for (int i = 0; i < mesh.indexCount; i++) {
				inFile >> mesh.indices[i];
			}
		}
	}

	Mesh* MeshLoader::Load(const char* path)
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
	}

	TexturedMesh* MeshLoader::LoadTextured(const char* path, Texture2D* texture) {
		TexturedMesh* tempMesh = new TexturedMesh();	//Create textured mesh
		tempMesh->mesh = new Mesh();		//Create mesh inside that textured mesh

		//Load mesh file
		ifstream inFile;
		inFile.open(path);
		if (!inFile.good()) {
			cerr << "Can't open textured mesh file " << path << endl;
			return nullptr;
		}

		//Load TexturedMesh's Mesh variables, and the TexCoords in TexturedMesh
		LoadVertices(inFile, *tempMesh->mesh);
		LoadTexCoords(inFile, *tempMesh);
		LoadNormals(inFile, *tempMesh->mesh);
		LoadIndices(inFile, *tempMesh->mesh);

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

		std::vector<Vertex*> tempVertices;
		std::vector<Vector3*> tempNormals;
		std::vector<TexCoord*> tempTexCoords;

		while (std::getline(inFile, line)) {
			//Create stringstream of current line
			std::stringstream curLine(line);
			//Current type of line it's on (represented by first characters of line)
			std::string curDataType;
			curLine >> curDataType;
			std::cout << curDataType << std::endl;

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
				for (int i = 0; i < 3; i++) {
					std::string data;
					curLine >> data;

					//Need to get data from XX/XX/XX into above arrays. Got to 'f' loading section on http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
				}
			}
			
		}

		return nullptr;
	}
}