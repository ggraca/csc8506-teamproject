#include "Material.h"



Material::Material()
{
	shader = nullptr;
	textureMatrix.ToIdentity();
}


Material::~Material()
{
}
