#pragma once
#include <map>
#include <vector>
#include <functional>
#include <string>
using std::map;
using std::vector;

#include "ShaderBase.h"

namespace NCL {

	typedef std::function<Rendering::ShaderBase*(const string& vertex, const string& fragment, const string& geometry, const string& domain, const string& hull)> APILoadShaderFunction;

	class ShaderLoader
	{
	public:
		static void RegisterAPILoadFunction(APILoadShaderFunction f);

		static Rendering::ShaderBase* LoadAPIShader(const string& vertex, const string& fragment, const string& geometry, const string& domain, const string& hull);

	protected:

		static APILoadShaderFunction apiFunction;

	};
}