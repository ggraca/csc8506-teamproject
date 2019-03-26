#pragma once

#include "../../Common/PixOpsFlags.h"
#include "glad\glad.h"

namespace NCL {
	namespace Rendering {
		class OGLPixOpsFLags : public PixOpsFlags
		{
		public:
			OGLPixOpsFLags() {};
			~OGLPixOpsFLags() {};

			void Init() override {
				SetFaceCulling(CULLFACE::BACK);

				SetDepthComparison(COMPARISON::LESS);
				SetDepthMask(true);

				SetStencilComparison(COMPARISON::NOCOMPARE, nullptr, nullptr);
				SetStencilMask(false);

				SetClearColor(Vec4(0.2f, 0.2f, 0.2f, 1.0f));
				SetColourMask(std::make_tuple(true, true, true, true));

				SetSourceFactor(BLEND::SRC_ALPHA);
				SetDestinationFactor(BLEND::ONE_MINUS_SRC_ALPHA);
			}

			void SetFaceCulling(CULLFACE cull) override {
				if (culling == CULLFACE::NOCULL) {
					glEnable(GL_CULL_FACE);
				}

				switch (cull)
				{
				case CULLFACE::NOCULL:
					glDisable(GL_CULL_FACE);
					break;
				case CULLFACE::FRONT:
					glCullFace(GL_FRONT);
					break;
				case CULLFACE::BACK:
					glCullFace(GL_BACK);
					break;
				default:
					std::cout << __FUNCTION__ << "called with invalid paramter" << std::endl;
					break;
				}

				culling = cull;
			}

			void SetDepthComparison(COMPARISON comp) override {
				if (depthComparison == COMPARISON::NOCOMPARE) {
					glEnable(GL_DEPTH_TEST);
				}

				switch (comp)
				{
				case COMPARISON::NOCOMPARE:
					glDisable(GL_DEPTH_TEST);
					break;
				case COMPARISON::NEVER:
					glDepthFunc(GL_NEVER);
					break;
				case COMPARISON::LESS:
					glDepthFunc(GL_LESS);
					break;
				case COMPARISON::EQUAL:
					glDepthFunc(GL_EQUAL);
					break;
				case COMPARISON::LEQUAL:
					glDepthFunc(GL_LEQUAL);
					break;
				case COMPARISON::GREATER:
					glDepthFunc(GL_GREATER);
					break;
				case COMPARISON::NOTEQUAL:
					glDepthFunc(GL_NOTEQUAL);
					break;
				case COMPARISON::GEQUAL:
					glDepthFunc(GL_GEQUAL);
					break;
				case COMPARISON::ALWAYS:
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
				if (stencilComparison == COMPARISON::NOCOMPARE) {
					glEnable(GL_STENCIL_TEST);
				}

				switch (comp)
				{
				case COMPARISON::NOCOMPARE:
					glDisable(GL_STENCIL_TEST);
					break;
				case COMPARISON::NEVER:
					glStencilFunc(GL_NEVER, *(int*)ref, *(unsigned int*)mask);
					break;
				case COMPARISON::LESS:
					glStencilFunc(GL_LESS, *(int*)ref, *(unsigned int*)mask);
					break;
				case COMPARISON::EQUAL:
					glStencilFunc(GL_EQUAL, *(int*)ref, *(unsigned int*)mask);
					break;
				case COMPARISON::LEQUAL:
					glStencilFunc(GL_LEQUAL, *(int*)ref, *(unsigned int*)mask);
					break;
				case COMPARISON::GREATER:
					glStencilFunc(GL_GREATER, *(int*)ref, *(unsigned int*)mask);
					break;
				case COMPARISON::NOTEQUAL:
					glStencilFunc(GL_NOTEQUAL, *(int*)ref, *(unsigned int*)mask);
					break;
				case COMPARISON::GEQUAL:
					glStencilFunc(GL_GEQUAL, *(int*)ref, *(unsigned int*)mask);
					break;
				case COMPARISON::ALWAYS:
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

			void SetClearColor(NCL::Maths::Vec4 color) override {
				glClearColor(color.x, color.y, color.z, color.w);
				clearColor = color;
			}

			void SetColourMask(ColorMask mask) override {
				bool r, g, b, a;
				std::tie(r, g, b, a) = mask;
				glColorMask(r, g, b, a);
			}
		protected:
			void SetBlendFunc() override {
				GLuint source;
				GLuint dest;

				switch (sourceFactor)
				{
				case BLEND::NOBLEND:
					source = GL_NONE;
					break;
				case BLEND::ZERO:
					source = GL_ZERO;
					break;
				case BLEND::ONE:
					source = GL_ONE;
					break;
				case BLEND::SRC_COLOR:
					source = GL_SRC_COLOR;
					break;
				case BLEND::ONE_MINUS_SRC_COLOR:
					source = GL_ONE_MINUS_SRC_COLOR;
					break;
				case BLEND::DST_COLOR:
					source = GL_DST_COLOR;
					break;
				case BLEND::ONE_MINUS_DST_COLOR:
					source = GL_ONE_MINUS_DST_COLOR;
					break;
				case BLEND::SRC_ALPHA:
					source = GL_SRC_ALPHA;
					break;
				case BLEND::ONE_MINUS_SRC_ALPHA:
					source = GL_ONE_MINUS_SRC_ALPHA;
					break;
				case BLEND::DST_ALPHA:
					source = GL_DST_ALPHA;
					break;
				case BLEND::ONE_MINUS_DST_ALPHA:
					source = GL_ONE_MINUS_DST_ALPHA;
					break;
				case BLEND::SRC_ALPHA_SATURATE:
					source = GL_SRC_ALPHA_SATURATE;
					break;
				default:
					std::cout << __FUNCTION__ << "called with invalid paramter" << std::endl;
					break;
				}

				switch (destinationFactor)
				{
				case BLEND::NOBLEND:
					dest = GL_NONE;
					break;
				case BLEND::ZERO:
					dest = GL_ZERO;
					break;
				case BLEND::ONE:
					dest = GL_ONE;
					break;
				case BLEND::SRC_COLOR:
					dest = GL_SRC_COLOR;
					break;
				case BLEND::ONE_MINUS_SRC_COLOR:
					dest = GL_ONE_MINUS_SRC_COLOR;
					break;
				case BLEND::DST_COLOR:
					dest = GL_DST_COLOR;
					break;
				case BLEND::ONE_MINUS_DST_COLOR:
					dest = GL_ONE_MINUS_DST_COLOR;
					break;
				case BLEND::SRC_ALPHA:
					dest = GL_SRC_ALPHA;
					break;
				case BLEND::ONE_MINUS_SRC_ALPHA:
					dest = GL_ONE_MINUS_SRC_ALPHA;
					break;
				case BLEND::DST_ALPHA:
					dest = GL_DST_ALPHA;
					break;
				case BLEND::ONE_MINUS_DST_ALPHA:
					dest = GL_ONE_MINUS_DST_ALPHA;
					break;
				case BLEND::SRC_ALPHA_SATURATE:
					dest = GL_SRC_ALPHA_SATURATE;
					break;
				default:
					std::cout << __FUNCTION__ << "called with invalid paramter" << std::endl;
					break;
				}
				if (source == GL_NONE || dest == GL_NONE) {
					glDisable(GL_BLEND);
				}
				else {
					glEnable(GL_BLEND);
					glBlendFunc(source, dest);
				}
			}
		};
	}
}