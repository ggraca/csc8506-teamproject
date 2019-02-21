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
Rendering::TextureBase* Assets::AssetManager::LoadTexture(const std::string& texturename, const std::string& filename) {
	Rendering::TextureBase* newTexture;
	newTexture = TextureLoader::LoadAPITexture(filename);
	getInstance().loadedTextures.insert(std::make_pair(texturename, newTexture));
	return newTexture;
}

void Assets::AssetManager::FlushTextures() {
	for (auto i = getInstance().loadedTextures.begin(); i != getInstance().loadedTextures.end(); i++)
	{
		delete(i->second);
	}
}

void Assets::AssetManager::FlushAssets() {
	FlushTextures();
}