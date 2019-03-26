#include "Material.h"

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
	colour = NCL::Maths::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Material::~Material()
{
}
