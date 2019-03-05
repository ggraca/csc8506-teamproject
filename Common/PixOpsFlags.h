#pragma once
#include <tuple>
#include "Vector3.h"
#include "Vector4.h"

namespace NCL {
	namespace Rendering {
		enum BLEND {
			NOBLEND = 0,
			ZERO = 1,
			ONE = 2,
			SRC_COLOR = 3,
			ONE_MINUS_SRC_COLOR = 4,
			DST_COLOR = 5,
			ONE_MINUS_DST_COLOR = 6,
			SRC_ALPHA = 7,
			ONE_MINUS_SRC_ALPHA = 8,
			DST_ALPHA = 9,
			ONE_MINUS_DST_ALPHA = 10,
			SRC_ALPHA_SATURATE = 11
		};

		enum CULLFACE {
			NOCULL = 0,
			FRONT = 1,
			BACK = 2
		};

		enum COMPARISON {
			NOCOMPARE = 0,
			NEVER = 1,
			LESS = 2,
			EQUAL = 3,
			LEQUAL = 4,
			GREATER = 5,
			NOTEQUAL = 6,
			GEQUAL = 7,
			ALWAYS = 8
		};

		class PixOpsFlags
		{
		public:
			typedef std::tuple<bool, bool, bool, bool> ColorMask;

			PixOpsFlags() {};

			virtual ~PixOpsFlags() = 0;

			virtual void SetFaceCulling(CULLFACE cull) = 0;
			CULLFACE GetFaceCulling() { return culling; }

			virtual void SetDepthComparison(COMPARISON comp) = 0;
			COMPARISON GetDepthComparison() { return depthComparison; }

			virtual void SetDepthMask(bool mask) = 0;
			bool GetDepthMask() { return depthMask; }

			virtual void SetStencilComparison(COMPARISON comp, void* ref, void* mask) = 0;
			COMPARISON GetStencilComparison() { return stencilComparison; }

			virtual void SetStencilMask(bool mask) = 0;
			bool GetStencilMask() { return stencilMask; }

			virtual void SetClearColor(NCL::Maths::Vector4 color) = 0;
			NCL::Maths::Vector4 GetClearColor() { return clearColor; }

			virtual void SetColourMask(ColorMask mask) = 0;
			ColorMask GetColourMask() { return colorMask; }

			virtual void SetSourceFactor(BLEND blend) {
				sourceFactor = blend;
				SetBlendFunc();
			}
			BLEND GetSourceFactor() { return sourceFactor; }

			virtual void SetDestinationFactor(BLEND blend) {
				destinationFactor = blend;
				SetBlendFunc();
			}
			BLEND GetDestinationFactor() { return destinationFactor; }

			virtual void Init() = 0;

		protected:
			CULLFACE culling;

			COMPARISON depthComparison;
			bool depthMask;

			COMPARISON stencilComparison;
			bool stencilMask;

			NCL::Maths::Vector4 clearColor;
			ColorMask colorMask;

			COMPARISON alphaComparison;
			BLEND sourceFactor;
			BLEND destinationFactor;
			virtual void SetBlendFunc() = 0;
		};

		inline PixOpsFlags::~PixOpsFlags() { }
	}
}