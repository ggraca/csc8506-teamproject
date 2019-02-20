#include "RenderObject.h"
#include "../Common/MeshGeometry.h"

using namespace NCL::CSC8503;
using namespace NCL;

RenderObject::RenderObject(Transform* parentTransform, MeshGeometry* mesh, Material* material) {
	this->transform	= parentTransform;
	this->mesh		= mesh;
	this->material = material;
	this->colour	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

RenderObject::~RenderObject() {

}