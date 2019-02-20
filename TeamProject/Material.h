#pragma once
#include "../Common/ShaderBase.h"
#include "../Common/TextureBase.h"
#include "../Common/Matrix4.h"
#include <vector>
#include <string>

using namespace NCL::Rendering;
using namespace NCL::Maths;

class Material
{
public:
	Material();
	~Material();

	ShaderBase* GetShader() const { return shader; }
	void SetShader(ShaderBase* s) { shader = s; }

	Matrix4 GetTextureMatrix() const { return textureMatrix; }
	void SetTextureMatrix(Matrix4 t) { textureMatrix = t; }

	void SetColour(const Vector4& c) { colour = c; }

	Vector4 GetColour() const { return colour; }

	void AddTextureParameter(std::string parameter, TextureBase* texture) {
		textureParameters.push_back(std::make_pair(parameter, texture));
	}

	const std::vector<std::pair<std::string, TextureBase*>>* GetTextureParameters() const { return &textureParameters; }

protected:
	ShaderBase* shader;
	std::vector<std::pair<std::string, TextureBase*>> textureParameters;

	//Think of better way to hold shader parameters
	Matrix4 textureMatrix;
	Vector4 colour;
};

