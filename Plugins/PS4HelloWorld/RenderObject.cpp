#include "RenderObject.h"
#include "../../Common/ShaderBase.h"
#include "../../Common/TextureBase.h"

using namespace NCL;
using namespace NCL::Rendering;
using namespace NCL::PS4;

RenderObjectPS4::RenderObjectPS4()
{
	mesh	= nullptr;
	shader	= nullptr;

	for (int i = 0; i < TEXTURE_COUNT; ++i) {
		textures[i] = nullptr;
	}
}

RenderObjectPS4::~RenderObjectPS4()
{

}

RenderObjectPS4::RenderObjectPS4(MeshGeometry* m, NCL::Rendering::ShaderBase* s, NCL::Rendering::TextureBase* t) {
	mesh		= m;
	shader		= s;
	textures[0] = t;
}

const Maths::Mat4& RenderObjectPS4::GetLocalTransform() const {
	return localTransform;
}

void	RenderObjectPS4::SetLocalTransform(const Maths::Mat4& mat) {
	localTransform = mat;
}