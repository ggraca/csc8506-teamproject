#include "RenderObject.h"


using namespace NCL::CSC8503;
using namespace NCL;

RenderObject::RenderObject(Transform* parentTransform, MeshGeometry* mesh, Material* material) {
	this->transform	= parentTransform;
	this->mesh		= mesh;
	this->material = material;
	SetMaterialInstanced();
}

RenderObject::~RenderObject() {
	if (materialInstanced) {
		delete material;
	}
}