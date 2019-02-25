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

Rendering::TextureBase* Assets::AssetManager::LoadTexture(const std::string& filename) {
	auto iter = GetInstance().loadedTextures.find(filename);

	if (iter != GetInstance().loadedTextures.end()) {
		return (*iter).second;
	}

	Rendering::TextureBase* newTexture;
	newTexture = TextureLoader::LoadAPITexture(filename);
	GetInstance().loadedTextures.insert(std::make_pair(filename, newTexture));
	return newTexture;
}

Rendering::Material* Assets::AssetManager::LoadMaterial(const std::string& materialname, Rendering::ShaderBase* shader) {
	auto iter = GetInstance().loadedMaterials.find(materialname);

	if (iter != GetInstance().loadedMaterials.end()) {
		return (*iter).second;
	}

	Rendering::Material* newMaterial;
	newMaterial->SetShader(shader);
	GetInstance().loadedMaterials.insert(std::make_pair(materialname, newMaterial));
	return newMaterial;
}

void Assets::AssetManager::FlushTextures() {
	for (auto i = GetInstance().loadedTextures.begin(); i != GetInstance().loadedTextures.end(); i++)
	{
		delete(i->second);
	}
	GetInstance().loadedTextures.clear();
}

void Assets::AssetManager::FlushShaders() {
	for (auto i = GetInstance().loadedShaders.begin(); i != GetInstance().loadedShaders.end(); i++)
	{
		delete(i->second);
	}
	GetInstance().loadedShaders.clear();
}

void Assets::AssetManager::FlushMaterials() {
	GetInstance().loadedMaterials.clear();
}

void Assets::AssetManager::FlushAssets() {
	FlushTextures();
	FlushShaders();
	FlushMaterials();
}