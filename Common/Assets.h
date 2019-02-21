#pragma once
#include <string>
#include <map>

#include "TextureLoader.h"
#include "Material.h"

namespace NCL {
	namespace Assets {
		const std::string SHADERDIR("../Assets/Shaders/");
		const std::string MESHDIR("../Assets/Meshes/");
		const std::string TEXTUREDIR("../Assets/Textures/");
		const std::string SOUNDSDIR("../Assets/Sounds/");
		const std::string FONTSSDIR("../Assets/Fonts/");
		const std::string DATADIR("../Assets/Data/");
		
		static std::map<std::string, Rendering::TextureBase*> loadedTextures;

		extern bool ReadTextFile(const std::string &filepath, std::string& result);
		//static inline bool ReadBinaryFile(const std::string &filepath, std::string& result);
		extern Rendering::TextureBase* LoadTexture(const std::string& texturename, const std::string& filename);
		extern bool FlushAssets();
	}
}
