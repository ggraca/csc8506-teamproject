#pragma once

#include "../../Common/PixOpsFlags.h"
#include "glad\glad.h"

class OGLPixOpsFLags : public PixOpsFlags
{
public:
	OGLPixOpsFLags() {};
	~OGLPixOpsFLags() override {};

	void SetFaceCulling(CULLFACE cull) override {
		if (culling == CULLFACE::NONE) {
			glEnable(GL_CULL_FACE);
		}

		switch (cull)
		{
		case PixOpsFlags::NONE:
			glDisable(GL_CULL_FACE);
			break;
		case PixOpsFlags::FRONT:
			glCullFace(GL_FRONT);
			break;
		case PixOpsFlags::BACK:
			glCullFace(GL_BACK);
			break;
		default:
			std::cout << __FUNCTION__ << "called with invalid paramter" << std::endl;
			break;
		}

		culling = cull;
	}

	void SetDepthComparison(COMPARISON comp) override {

	}

	void SetDepthMask(bool mask) = 0;

	void SetStencilComparison(COMPARISON comp) = 0;

	void SetStencilMask(bool mask) = 0;

	void SetClearColor(NCL::Maths::Vector4 color) = 0;

	void SetColourMask(ColorMask mask) = 0;

	void SetAlphaBlend(BLEND blend) = 0;
};