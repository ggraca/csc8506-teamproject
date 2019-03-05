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
		alphaBlend = BLEND::ZERO;
	};

	virtual ~PixOpsFlags() = 0;

	
	enum BLEND {
		ZERO = 0,
		ONE = 1,
		SRC_COLOR = 2,
		ONE_MINUS_SRC_COLOR = 3,
		DST_COLOR = 4,
		ONE_MINUS_DST_COLOR = 5,
		SRC_ALPHA = 6,
		ONE_MINUS_SRC_ALPHA = 7,
		DST_ALPHA = 8,
		ONE_MINUS_DST_ALPHA = 9,
		SRC_ALPHA_SATURATE = 10
	};

	enum CULLFACE {
		NONE = 0,
		FRONT = 1,
		BACK = 2
	};

	enum COMPARISON {
		NONE = 0,
		NEVER = 1,
		LESS = 2,
		EQUAL = 3,
		LEQUAL = 4,
		GREATER = 5,
		NOTEQUAL = 6,
		GEQUAL = 7,
		ALWAYS = 8
	};

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