#pragma once
#include "../Common/ShaderBase.h"
#include "../Common/TextureBase.h"
#include "../Common/Matrix4.h"
#include <vector>
#include <string>

namespace NCL {
	namespace Rendering {
		class Material
		{
		public:
			Material(Material& mat);
			Material();
			~Material();

			ShaderBase* GetShader() const { return shader; }
			void SetShader(ShaderBase* s) { shader = s; }

			NCL::Maths::Mat4 GetTextureMatrix() const { return textureMatrix; }
			void SetTextureMatrix(NCL::Maths::Mat4 t) { textureMatrix = t; }

			void SetColour(const  NCL::Maths::Vec4& c) { colour = c; }

			NCL::Maths::Vec4 GetColour() const { return colour; }

			void AddTextureParameter(std::string parameter, TextureBase* texture) {
				textureParameters.push_back(std::make_pair(parameter, texture));
			}

			const std::vector<std::pair<std::string, TextureBase*>>* GetTextureParameters() const { return &textureParameters; }

		protected:
			ShaderBase* shader;
			std::vector<std::pair<std::string, TextureBase*>> textureParameters;

			//Think of better way to hold shader parameters
			NCL::Maths::Mat4 textureMatrix;
			NCL::Maths::Vec4 colour;
		};
	}
}

