#ifdef _WIN32

#include "ShaderLoader.h"
#include <iostream>

#include <filesystem>
#include "Assets.h"
using namespace std::experimental::filesystem::v1;

using namespace NCL;
using namespace Rendering;

APILoadShaderFunction ShaderLoader::apiFunction = nullptr;

void ShaderLoader::RegisterAPILoadFunction(APILoadShaderFunction f) {
	if (apiFunction) {
		std::cout << __FUNCTION__ << " replacing previously defined API function." << std::endl;
	}
	apiFunction = f;
}

Rendering::ShaderBase* ShaderLoader::LoadAPIShader(const string& vertex, const string& fragment, const string& geometry, const string& domain, const string& hull) {
	if (apiFunction == nullptr) {
		std::cout << __FUNCTION__ << " no API Function has been defined!" << std::endl;
		return nullptr;
	}
	return apiFunction(vertex, fragment, geometry, domain, hull);
}
#endif