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

	void SetStencilMask(bool mask) override {
		if (mask) {
			glStencilMask(GL_TRUE);
		}
		else {
			glStencilMask(GL_FALSE);
		}

		stencilMask = mask;
	}

	void SetClearColor(NCL::Maths::Vector4 color) override {
		glClearColor(color.x, color.y, color.z, color.w);
		clearColor = color;
	}

	void SetColourMask(ColorMask mask) override {
		bool r, g, b, a;
		std::tie(r, g, b, a) = mask;
		glColorMask(r, g, b, a);
	}

	void SetBlendFunc() override {
		GLuint source;
		GLuint dest;

		switch (sourceFactor)
		{
		case PixOpsFlags::ZERO:
			source = GL_ZERO;
			break;
		case PixOpsFlags::ONE:
			source = GL_ONE;
			break;
		case PixOpsFlags::SRC_COLOR:
			source = GL_SRC_COLOR;
			break;
		case PixOpsFlags::ONE_MINUS_SRC_COLOR:
			source = GL_ONE_MINUS_SRC_COLOR;
			break;
		case PixOpsFlags::DST_COLOR:
			source = GL_DST_COLOR;
			break;
		case PixOpsFlags::ONE_MINUS_DST_COLOR:
			source = GL_ONE_MINUS_DST_COLOR;
			break;
		case PixOpsFlags::SRC_ALPHA:
			source = GL_SRC_ALPHA;
			break;
		case PixOpsFlags::ONE_MINUS_SRC_ALPHA:
			source = GL_ONE_MINUS_SRC_ALPHA;
			break;
		case PixOpsFlags::DST_ALPHA:
			source = GL_DST_ALPHA;
			break;
		case PixOpsFlags::ONE_MINUS_DST_ALPHA:
			source = GL_ONE_MINUS_DST_ALPHA;
			break;
		case PixOpsFlags::SRC_ALPHA_SATURATE:
			source = GL_SRC_ALPHA_SATURATE;
			break;
		default:
			std::cout << __FUNCTION__ << "called with invalid paramter" << std::endl;
			break;
		}

		switch (destinationFactor)
		{
		case PixOpsFlags::ZERO:
			dest = GL_ZERO;
			break;
		case PixOpsFlags::ONE:
			dest = GL_ONE;
			break;
		case PixOpsFlags::SRC_COLOR:
			dest = GL_SRC_COLOR;
			break;
		case PixOpsFlags::ONE_MINUS_SRC_COLOR:
			dest = GL_ONE_MINUS_SRC_COLOR;
			break;
		case PixOpsFlags::DST_COLOR:
			dest = GL_DST_COLOR;
			break;
		case PixOpsFlags::ONE_MINUS_DST_COLOR:
			dest = GL_ONE_MINUS_DST_COLOR;
			break;
		case PixOpsFlags::SRC_ALPHA:
			dest = GL_SRC_ALPHA;
			break;
		case PixOpsFlags::ONE_MINUS_SRC_ALPHA:
			dest = GL_ONE_MINUS_SRC_ALPHA;
			break;
		case PixOpsFlags::DST_ALPHA:
			dest = GL_DST_ALPHA;
			break;
		case PixOpsFlags::ONE_MINUS_DST_ALPHA:
			dest = GL_ONE_MINUS_DST_ALPHA;
			break;
		case PixOpsFlags::SRC_ALPHA_SATURATE:
			dest = GL_SRC_ALPHA_SATURATE;
			break;
		default:
			std::cout << __FUNCTION__ << "called with invalid paramter" << std::endl;
			break;
		}

		glBlendFunc(source, dest);
	}
};