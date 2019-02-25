#include "Material.h"

using namespace NCL;
using namespace Rendering;

Material::Material(Material& mat)
{
	this->shader = mat.GetShader();
	this->textureMatrix = mat.GetTextureMatrix();
	this->colour = mat.GetColour();

	for (auto  i = mat.GetTextureParameters()->begin(); i != mat.GetTextureParameters()->end(); i++)
	{
		textureParameters.push_back(*i);
	}
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
