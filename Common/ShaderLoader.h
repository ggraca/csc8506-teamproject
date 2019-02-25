#pragma once
#include <map>
#include <vector>
#include <functional>
#include <string>
using std::map;
using std::vector;

#include "ShaderBase.h"

namespace NCL {

	typedef std::function<bool(const string& vertex, const string& fragment, const string& geometry, const string& domain, const string& hull)> ShaderLoadFunction;

	typedef std::function<Rendering::ShaderBase*(const std::string& filename)> APILoadFunction;

	class ShaderLoader
	{
	public:

		static bool LoadTexture(const std::string& filename, char*& outData, int& width, int &height, int &channels, int&flags);

		static void RegisterTextureLoadFunction(ShaderLoadFunction f, const std::string&fileExtension);

		static void RegisterAPILoadFunction(APILoadFunction f);

		static Rendering::ShaderBase* LoadAPITexture(const std::string&filename);

	protected:
		static std::map<std::string, ShaderLoadFunction> fileHandlers;

		static APILoadFunction apiFunction;

	};
}