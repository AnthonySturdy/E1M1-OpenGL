#pragma once

#include "Structures.h"

namespace MeshLoader
{
	Mesh* Load(const char* path);
	TexturedMesh* LoadTextured(const char* path, Texture2D* texture);
	TexturedMesh* LoadOBJ(const char* path, Texture2D* texture);
};