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
		
		extern bool ReadTextFile(const std::string &filepath, std::string& result);
		
		class AssetManager {
		public:
			AssetManager(AssetManager const&) = delete;
			void operator=(AssetManager const&) = delete;

			static AssetManager& GetInstance()
			{
				static AssetManager instance;
				return instance;
			}

			static Rendering::TextureBase* LoadTexture(const std::string& texturename, const std::string& filename);

			static void FlushTextures();
			static void FlushShaders();
			static void FlushMaterials();
			static void FlushAssets();
		protected:
			std::map<std::string, Rendering::TextureBase*> loadedTextures;
			std::map<std::string, Rendering::ShaderBase*> loadedShaders;
			std::map<std::string, Rendering::Material*> loadedMaterials;
		private:
			AssetManager() {};
		};

		//static inline bool ReadBinaryFile(const std::string &filepath, std::string& result);
	}
}
