#include "Material.h"

using namespace NCL;
using namespace Rendering;

Material::Material()
{
	shader = nullptr;
	textureMatrix.ToIdentity();
	colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}


Material::~Material()
{
}
