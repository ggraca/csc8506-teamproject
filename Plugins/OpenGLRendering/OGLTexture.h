#pragma once
#include "../../Common/TextureBase.h"
#include "glad\glad.h"

#include <string>
#include <vector>

namespace NCL {
	namespace Rendering {
		class OGLTexture : public TextureBase
		{
		public:
			friend class OGLRenderer;

			~OGLTexture();

			static TextureBase* RGBATextureFromData(char* data, int width, int height, int channels);

			static TextureBase* CubeTextureFromData(char** data, int width, int height, int channels);

			static TextureBase* RGBATextureFromFilename(const std::string&name);

			static TextureBase* CubeTextureFromFilename(const std::vector<std::string>& faces);

			GLuint GetObjectID() const	{
				return texID;
			}
		protected:			
			OGLTexture();
			GLuint texID;
		};
	}
}

