#pragma once
#include "../Common/ShaderBase.h"
#include "../Common/TextureBase.h"
#include "../Common/Matrix4.h"
#include <vector>
#include <string>

using namespace NCL::Maths;

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

			Matrix4 GetTextureMatrix() const { return textureMatrix; }
			void SetTextureMatrix(Matrix4 t) { textureMatrix = t; }

			void SetColour(const Vector4& c) { colour = c; }

			Vector4 GetColour() const { return colour; }

			void AddTextureParameter(std::string parameter, TextureBase* texture) {
				bool contains = false;
				for (size_t i = 0; i < textureParameters.size(); i++)
				{
					if (textureParameters[i].first == parameter) {
						textureParameters[i].second = texture;
						contains = true;
					}
				}
				if (!contains) {
					textureParameters.push_back(std::make_pair(parameter, texture));
				}
			}

			void InitBasicTextureParams();

			const std::vector<std::pair<std::string, TextureBase*>>* GetTextureParameters() const { return &textureParameters; }

		protected:
			ShaderBase* shader;
			std::vector<std::pair<std::string, TextureBase*>> textureParameters;

			//Think of better way to hold shader parameters
			Matrix4 textureMatrix;
			Vector4 colour;
		};
	}
}

