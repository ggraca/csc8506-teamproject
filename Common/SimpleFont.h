#pragma once
#ifdef _WIN32
#include <string>
#include <vector>
#include "TextureBase.h"

namespace NCL {
	namespace Maths {
		class Vec2;
		class Vec3;
		class Vec4;
	}
	namespace Rendering {
		class SimpleFont
		{
		public:
			SimpleFont(const std::string&fontName, const std::string&texName);
			~SimpleFont();

			int BuildVerticesForString(std::string &text, Maths::Vec2&startPos, Maths::Vec4&colour, std::vector<Maths::Vec3>&positions, std::vector<Maths::Vec2>&texCoords, std::vector<Maths::Vec4>&colours, float size = 1.0f);

			const TextureBase* GetTexture() const {
				return texture;
			}

		protected:
			//matches stbtt_bakedchar
			struct FontChar {
				unsigned short x0;
				unsigned short y0;
				unsigned short x1;
				unsigned short y1;
				float xOff;
				float yOff;
				float xAdvance;
			};

			FontChar*		allCharData;
			TextureBase*	texture;

			int startChar;
			int numChars;

			float texWidth;
			float texHeight;
		};
	}
}
#endif