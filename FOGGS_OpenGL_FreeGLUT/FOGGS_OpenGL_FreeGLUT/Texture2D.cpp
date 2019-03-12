#include "Texture2D.h"

Texture2D::Texture2D() {

}

Texture2D::~Texture2D() {

}

bool Texture2D::LoadFromRAW(const char* path, int w, int h) {
	char* tempTextureData;
	int fileSize;
	std::ifstream inFile;
	
	_width = w; _height = h;

	inFile.open(path, std::ios::binary);
	if (!inFile.good()) {
		std::cerr << "Can't open texture file " << path << std::endl;
		return false;
	}

	inFile.seekg(0, std::ios::end);	//Seek to the end of the file
	fileSize = (int)inFile.tellg();	//Get current position in file (the end), which gives us the total file size;
	tempTextureData = new char[fileSize];	//Create new array to store data
	inFile.seekg(0, std::ios::beg);	//Seek back to beginning of file
	inFile.read(tempTextureData, fileSize);	//Read all of the data in one go

	inFile.close();

	std::cout << path << " loaded" << std::endl;

	glGenTextures(1, &_ID);	//Get next texture ID
	glBindTexture(GL_TEXTURE_2D, _ID);	//Bind the texture to the ID
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

	delete[] tempTextureData;

	return true;
}

bool Texture2D::LoadFromData(char* data, int w, int h) {
	glGenTextures(1, &_ID);	//Get next texture ID
	glBindTexture(GL_TEXTURE_2D, _ID);	//Bind the texture to the ID
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	return true;
}
