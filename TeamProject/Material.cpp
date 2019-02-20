#include "Material.h"



Material::Material()
{
	shader = nullptr;
	textureMatrix.ToIdentity();
	colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}


Material::~Material()
{
}
