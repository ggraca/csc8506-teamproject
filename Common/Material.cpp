#include "Material.h"
#include "Assets.h"

using namespace NCL;
using namespace Rendering;

Material::Material(Material& mat)
{
	this->shader = mat.GetShader();
	this->textureMatrix = mat.GetTextureMatrix();
	this->colour = mat.GetColour();
	this->textureParameters = std::vector<std::pair<std::string, TextureBase*>>(*mat.GetTextureParameters());
}

Material::Material()
{
	shader = nullptr;
	textureMatrix.ToIdentity();
	colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

Material::~Material()
{
}

void Material::InitBasicTextureParams() {
	bool containsDiffuse = false;
	bool containsBump = false;
	bool containsRoughness = false;
	bool containsMetalness = false;
	bool containsAO = false;

	for (size_t i = 0; i < textureParameters.size(); i++)
	{
		if (textureParameters[i].first == "diffuseTex") {
			containsDiffuse = true;
		}
		else if (textureParameters[i].first == "bumpTex") {
			containsBump = true;
		}
		else if (textureParameters[i].first == "roughnessTex") {
			containsRoughness = true;
		}
		else if (textureParameters[i].first == "metalnessTex") {
			containsMetalness = true;
		}
		else if (textureParameters[i].first == "aoTex") {
			containsAO = true;
		}
	}
	if (!containsDiffuse) {
		AddTextureParameter("diffuseTex", Assets::AssetManager::LoadTexture("white.jpg"));
	}
	if (!containsBump) {
		AddTextureParameter("bumpTex", Assets::AssetManager::LoadTexture("blue.jpg"));
	}
	if (!containsRoughness) {
		AddTextureParameter("roughnessTex", Assets::AssetManager::LoadTexture("white.jpg"));
	}
	if (!containsMetalness) {
		AddTextureParameter("metalnessTex", Assets::AssetManager::LoadTexture("black.jpg"));
	}
	if (!containsAO) {
		AddTextureParameter("aoTex", Assets::AssetManager::LoadTexture("white.jpg"));
	}
}
