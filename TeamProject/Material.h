#pragma once
#include "../Common/ShaderBase.h"
#include "../Common/TextureBase.h"
#include <vector>
#include <string>

using namespace NCL::Rendering;

class Material
{
public:
	Material();
	~Material();

	ShaderBase* GetShader() const { return shader; }
	void SetShader(ShaderBase* s) { shader = s; }

	float GetTiling() const { return tiling; }
	void SetTiling(float t) { tiling = t; }

	void AddTextureParameter(std::string parameter, TextureBase* texture) {
		textureParameters.push_back(std::make_pair(parameter, texture));
	}

	const std::vector<std::pair<std::string, TextureBase*>>* GetTextureParameters() const { return &textureParameters; }

protected:
	ShaderBase* shader;
	std::vector<std::pair<std::string, TextureBase*>> textureParameters;

	//Think of better way to hold shader parameters
	float tiling = 1.0f;
};

