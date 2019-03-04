#pragma once
#include <tuple>
#include "Vector3.h"
#include "Vector4.h"

class PixOpsFlags
{
public:
	typedef std::tuple<bool, bool, bool, bool> ColorMask;

	PixOpsFlags() {
		culling = CULLFACE::NONE;

		depthComparison = COMPARISON::NEVER;
		depthMask = false;

		stencilComparison = COMPARISON::NEVER;
		stencilMask = false;

		NCL::Maths::Vector4 clearColor = NCL::Maths::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		colorMask = ColorMask(true, true, true, true);

		alphaComparison = COMPARISON::NEVER;
		alphaBlend = BLEND::GL_ZERO;
	};

	virtual ~PixOpsFlags() = 0;

	
	enum BLEND {
		GL_ZERO = 0,
		GL_ONE = 1,
		GL_SRC_COLOR = 2,
		GL_ONE_MINUS_SRC_COLOR = 3,
		GL_DST_COLOR = 4,
		GL_ONE_MINUS_DST_COLOR = 5,
		GL_SRC_ALPHA = 6,
		GL_ONE_MINUS_SRC_ALPHA = 7,
		GL_DST_ALPHA = 8,
		GL_ONE_MINUS_DST_ALPHA = 9,
		GL_SRC_ALPHA_SATURATE = 10
	};

	enum CULLFACE {
		NONE = 0,
		FRONT = 1,
		BACK = 2
	};

	enum COMPARISON {
		NEVER = 0,
		LESS = 1,
		EQUAL = 2,
		LEQUAL = 3,
		GREATER = 4,
		NOTEQUAL = 5,
		GEQUAL = 6,
		ALWAYS = 7
	};

	virtual void SetFaceCulling(CULLFACE cull) = 0;
	CULLFACE GetFaceCulling() { return culling; }

	virtual void SetDepthComparison(COMPARISON comp) = 0;
	COMPARISON GetDepthComparison() { return depthComparison; }

	virtual void SetDepthMask(bool mask) = 0;
	bool GetDepthMask() { return depthMask; }

	virtual void SetStencilComparison(COMPARISON comp) = 0;
	COMPARISON GetStencilComparison() { return stencilComparison; }

	virtual void SetStencilMask(bool mask) = 0;
	bool GetStencilMask() { return stencilMask; }

	virtual void SetClearColor(NCL::Maths::Vector4 color) = 0;
	NCL::Maths::Vector4 GetClearColor() { return clearColor; }

	virtual void SetColourMask(ColorMask mask) = 0;
	ColorMask GetColourMask() { return colorMask; }

	virtual void SetAlphaBlend(BLEND blend) = 0;
	BLEND GetAlphaBlend() { return alphaBlend; }

protected:
	CULLFACE culling;

	COMPARISON depthComparison;
	bool depthMask;

	COMPARISON stencilComparison;
	bool stencilMask;

	NCL::Maths::Vector4 clearColor;
	ColorMask colorMask;

	COMPARISON alphaComparison;
	BLEND alphaBlend;

};