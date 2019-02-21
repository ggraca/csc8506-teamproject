#include "Assets.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace NCL;

bool Assets::ReadTextFile(const std::string &filepath, std::string& result) {
	std::ifstream file(filepath, std::ios::in);
	if (file) {
		std::ostringstream stream;

		stream << file.rdbuf();

		result = stream.str();

		return true;
	}
	else {
		std::cout << __FUNCTION__ << " can't read file " << filepath << std::endl;
		return false;
	}
}
Rendering::TextureBase* Assets::LoadTexture(const std::string& texturename, const std::string& filename) {
	Rendering::TextureBase* newTexture;
	newTexture = TextureLoader::LoadAPITexture(filename);
	loadedTextures.insert(std::make_pair(texturename, newTexture));
	return newTexture;
}

bool Assets::FlushAssets() {
	for (auto i = loadedTextures.begin(); i != loadedTextures.end(); i++)
	{
		delete(i->second);
	}
	return true;
}