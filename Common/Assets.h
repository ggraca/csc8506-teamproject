#pragma once
#include <string>
#include <map>

#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "Material.h"
#include "MeshGeometry.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"


class OBJGeometry;

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

			static Rendering::TextureBase* LoadTexture(const std::string& filename);
			static Rendering::ShaderBase* LoadShader(const std::string& shadername, const string& vertex, const string& fragment, const string& geometry = "", const string& domain = "", const string& hull = "");
			static MeshGeometry* LoadMesh(const std::string& filename);
			static OBJGeometry* LoadOBJ(const std::string& filename);
			static Rendering::Material* LoadMaterial(const std::string& materialname, Rendering::ShaderBase* shader);

			static void FlushTextures();
			static void FlushShaders();
			static void FlushMaterials();
			static void FlushAssets();

			static std::map<std::string, OBJGeometry*>* GetOBJMeshes() { return &GetInstance().loadedOBJs; }

		protected:
			std::map<std::string, Rendering::TextureBase*> loadedTextures;
			std::map<std::string, MeshGeometry*> loadedMeshes;
			std::map<std::string, OBJGeometry*> loadedOBJs;
			std::map<std::string, Rendering::ShaderBase*> loadedShaders;
			std::map<std::string, Rendering::Material*> loadedMaterials;
			
			// For later use when meshes and sounds can be loaded from one place.
			// Also create add and flush functions
			// std::map<std::string, Rendering::Material*> loadedSounds;

		private:
			AssetManager() {};
		};
	}
}
