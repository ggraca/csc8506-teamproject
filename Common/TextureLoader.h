#pragma once
#include <map>
#include <vector>
#include <functional>
#include <string>
using std::map;
using std::vector;

#include "TextureBase.h"

namespace NCL {

	typedef std::function<bool(const std::string& filename, char*& outData, int& width, int &height, int &channels, int&flags)> TextureLoadFunction;

	typedef std::function<Rendering::TextureBase*(const std::string& filename)> APILoadFunction;

	typedef std::function<Rendering::TextureBase*(const std::vector<std::string>& faces)> APILoadFunctionCube;

	class TextureLoader
	{
	public:

		static bool LoadTexture(const std::string& filename, char*& outData, int& width, int &height, int &channels, int&flags);

		static void RegisterTextureLoadFunction(TextureLoadFunction f, const std::string&fileExtension);

		static void RegisterAPILoadFunction(APILoadFunction f);

		static void RegisterAPILoadCubeFunction(APILoadFunctionCube f);

		static Rendering::TextureBase* LoadAPITexture(const std::string&filename);

		static Rendering::TextureBase* LoadAPICubeTexture(const std::vector<std::string>& faces);

	protected:

		static std::string GetFileExtension(const std::string& fileExtension);

		static std::map<std::string, TextureLoadFunction> fileHandlers;

		static APILoadFunction apiFunction;

		static APILoadFunctionCube apiFunctionCube;

	};
}

