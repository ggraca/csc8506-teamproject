#include "Assets.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "OBJGeometry.h"

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

Rendering::ShaderBase* Assets::AssetManager::LoadShader(const string& shadername, const string& vertex, const string& fragment, const string& geometry, const string& domain, const string& hull) {
	auto iter = GetInstance().loadedShaders.find(shadername);

	if (iter != GetInstance().loadedShaders.end()) {
		return (*iter).second;
	}

	Rendering::ShaderBase* newShader;
	newShader = ShaderLoader::LoadAPIShader(vertex, fragment, geometry, domain, hull);
	GetInstance().loadedShaders.insert(std::make_pair(shadername, newShader));
	return newShader;
}

MeshGeometry* Assets::AssetManager::LoadMesh(const std::string& filename, bool tesselated) {
	auto iter = GetInstance().loadedMeshes.find(filename);

	if (!tesselated) {
		if (iter != GetInstance().loadedMeshes.end()) {
			return (*iter).second;
		}
	}

	// TODO: Make this compatible with PS4
	MeshGeometry* newMesh = (MeshGeometry*) new Rendering::OGLMesh(filename);
	// .msh files have always triangles as faces
	if (tesselated) {
		newMesh->SetPrimitiveType(GeometryPrimitive::Patches);
	}
	else {
		newMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	}
	newMesh->UploadToGPU();
	GetInstance().loadedMeshes.insert(std::make_pair(filename, newMesh));
	return newMesh;
}

OBJGeometry* Assets::AssetManager::LoadOBJ(const std::string& filename) {
	auto iter = GetInstance().loadedOBJs.find(filename);

	if (iter != GetInstance().loadedOBJs.end()) {
		return (*iter).second;
	}

	OBJGeometry* newOBJ = new OBJGeometry(Assets::MESHDIR + filename);
	GetInstance().loadedOBJs.insert(std::make_pair(filename, newOBJ));
	return newOBJ;

}

Rendering::Material* Assets::AssetManager::LoadMaterial(const std::string& materialname, Rendering::ShaderBase* shader) {
	auto iter = GetInstance().loadedMaterials.find(materialname);

	if (iter != GetInstance().loadedMaterials.end()) {
		return (*iter).second;
	}

	Rendering::Material* newMaterial = new Rendering::Material();
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