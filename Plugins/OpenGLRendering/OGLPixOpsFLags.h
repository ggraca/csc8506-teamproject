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
		if (depthComparison == COMPARISON::NONE) {
			glEnable(GL_DEPTH_TEST);
		}

		switch (comp)
		{
		case PixOpsFlags::NEVER:
			glDisable(GL_DEPTH_TEST);
			break;
		case PixOpsFlags::LESS:
			glDepthFunc(GL_LESS);
			break;
		case PixOpsFlags::EQUAL:
			glDepthFunc(GL_EQUAL);
			break;
		case PixOpsFlags::LEQUAL:
			glDepthFunc(GL_LEQUAL);
			break;
		case PixOpsFlags::GREATER:
			glDepthFunc(GL_GREATER);
			break;
		case PixOpsFlags::NOTEQUAL:
			glDepthFunc(GL_NOTEQUAL);
			break;
		case PixOpsFlags::GEQUAL:
			glDepthFunc(GL_GEQUAL);
			break;
		case PixOpsFlags::ALWAYS:
			glDepthFunc(GL_ALWAYS);
			break;
		default:
			std::cout << __FUNCTION__ << "called with invalid paramter" << std::endl;
			break;
		}

		depthComparison = comp;
	}

	void SetDepthMask(bool mask) override {
		if (mask) {
			glDepthMask(GL_TRUE);
		}
		else {
			glDepthMask(GL_FALSE);
		}

		depthMask = mask;
	}

	void SetStencilComparison(COMPARISON comp, void* ref, void* mask) override {
		if (stencilComparison == COMPARISON::NONE) {
			glEnable(GL_STENCIL_TEST);
		}

		switch (comp)
		{
		case PixOpsFlags::NEVER:
			glDisable(GL_STENCIL_TEST);
			break;
		case PixOpsFlags::LESS:
			glStencilFunc(GL_LESS, *(int*)ref, *(unsigned int*)mask);
			break;
		case PixOpsFlags::EQUAL:
			glStencilFunc(GL_EQUAL, *(int*)ref, *(unsigned int*)mask);
			break;
		case PixOpsFlags::LEQUAL:
			glStencilFunc(GL_LEQUAL, *(int*)ref, *(unsigned int*)mask);
			break;
		case PixOpsFlags::GREATER:
			glStencilFunc(GL_GREATER, *(int*)ref, *(unsigned int*)mask);
			break;
		case PixOpsFlags::NOTEQUAL:
			glStencilFunc(GL_NOTEQUAL, *(int*)ref, *(unsigned int*)mask);
			break;
		case PixOpsFlags::GEQUAL:
			glStencilFunc(GL_GEQUAL, *(int*)ref, *(unsigned int*)mask);
			break;
		case PixOpsFlags::ALWAYS:
			glStencilFunc(GL_ALWAYS, *(int*)ref, *(unsigned int*)mask);
			break;
		default:
			std::cout << __FUNCTION__ << "called with invalid paramter" << std::endl;
			break;
		}

		stencilComparison = comp;
	}

	void SetStencilMask(bool mask) = 0;

	void SetClearColor(NCL::Maths::Vector4 color) = 0;

	void SetColourMask(ColorMask mask) = 0;

	void SetAlphaBlend(BLEND blend) = 0;
};