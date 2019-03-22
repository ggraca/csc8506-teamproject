#pragma once

#ifdef _WIN32
#include "../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#endif

#ifdef __ORBIS__
#include "../Plugins/PlayStation4/PS4RendererBase.h"
#include "../Plugins/PlayStation4/PS4Shader.h"
#include "../Plugins/PlayStation4/PS4Mesh.h"
#endif

#include "../Common/TextureLoader.h"

#include "GameWorld.h"
#include "HUDObject.h"
#include "CameraControl.h"
#include "RenderObject.h"

class Light;
class HUDObject;

namespace NCL {
	namespace CSC8503 {
		class RenderObject;
		class ParticleSystem;
		class PixOpsFlags;
		#ifdef _WIN32
		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer();
			~GameTechRenderer();

			void SetGameWorld(GameWorld* gw) { gameWorld = gw; }
			int GetRendererWidth() const { return currentWidth; }
			int GetRendererHeight() const { return currentHeight; }
			int GetVertsDrawn() const { return vertsDrawn; }
			int GetShadowCasters() const { return shadowCasters; }

			void SetLightMesh(OGLMesh* mesh) { lightSphere = mesh; }
			
			TextureBase* skybox;

			//HUD
			void AddHUDObjects();
			void WeaponState(int index, bool state);
			float health = 1; //(100%);
			//void UpdateHealthQuad();

			Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5, 0.5, 0.5)) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));

		protected:
			void RenderFrame()	override;
			void GenBuffers();
			void RenderHUD();

			OGLShader*		defaultShader;

			GameWorld*	gameWorld;

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderSkybox();
			void RenderCamera();
			void RenderLights();
			void CombineBuffers();
			void RenderParticleSystems();
			void RenderPostProcess();
			void PresentScene();

			//void SetupDebugMatrix(OGLShader*s) override;

			vector<const RenderObject*> activeObjects;
			vector<const Light*> activeLights;
			vector<ParticleSystem*> activeParticleSystems;
			vector<ShaderBase*> postProcessShaders;

			//shadow mapping things
			ShaderBase*	shadowShader;
			TextureBase* shadowTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			ShaderBase* skyBoxShader;

			GLuint gBufferFBO; // FBO for our G- Buffer pass
			TextureBase* gBufferDepthTex; // Depth goes here
			TextureBase* gBufferColourTex; // Albedo goes here
			TextureBase* gBufferNormalTex; // Normals go here
			TextureBase* gBufferSpecularTex; // Specular goes here

			GLuint lightFBO; // FBO for our lighting pass
			TextureBase* lightEmissiveTex; // emissive lighting
			TextureBase* lightSpecularTex; // specular lighting

			GLuint postFBO; // FBO for our post process pass
			TextureBase* postTexture[2]; // post process texture [0] and [1]
			int lastRendererdPostTex = 0;
			bool DoPostProcess = true;

			ShaderBase* combineShader;
			ShaderBase* presentShader;
			ShaderBase* pointLightShader;
			ShaderBase* directionalLightShader;
			ShaderBase* particleShader;
			ShaderBase* hudShader;
			OGLMesh* lightSphere;
			OGLMesh* screenQuad;

			GLuint hudTex;
			vector<HUDObject*> hudObjects;
			
			Vector4 ambientColour = Vector4(0.2f, 0.2f, 0.2f, 1.0f);

			int vertsDrawn = 0;
			int shadowCasters = 0;
			bool drawShadows = true;
		};
		#endif

		#ifdef __ORBIS__
		class GameTechRenderer : public PS4::PS4RendererBase {
		public:
			GameTechRenderer(GameWorld& world);
			~GameTechRenderer();

			int GetRendererWidth() const { return currentWidth; }
			int GetRendererHeight() const { return currentHeight; }
			int GetVertsDrawn() const { return vertsDrawn; }
			int GetShadowCasters() const { return shadowCasters; }

			void SetLightMesh(PS4Mesh* mesh) { lightSphere = mesh; }

			PS4Texture* skybox;

			//HUD
			void AddHUDObjects();
			void WeaponState(int index, bool state);
			float health = 1; //(100%);
			//void UpdateHealthQuad();

			Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5, 0.5, 0.5)) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));

		protected:
			void RenderFrame()	override;
			void GenBuffers();
			void RenderHUD();

			PS4Shader*		defaultShader;

			GameWorld&	gameWorld;

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderSkybox();
			void RenderCamera();
			void RenderLights();
			void CombineBuffers();

			//void SetupDebugMatrix(OGLShader*s) override;

			vector<const RenderObject*> activeObjects;

			//shadow mapping things
			PS4Shader*	shadowShader;
			PS4Texture*		shadowTex;
			sce::Gnm::RenderTarget		shadowFBO; //rendertargets are used instead of FBOs
			Matrix4     shadowMatrix;

			PS4Shader* skyBoxShader;

			NCL::PS4::PS4ScreenBuffer gBufferFBO; // FBO for our G- Buffer pass
			PS4Texture* gBufferDepthTex; // Depth goes here
			PS4Texture* gBufferColourTex; // Albedo goes here
			PS4Texture* gBufferNormalTex; // Normals go here
			PS4Texture* gBufferSpecularTex; // Specular goes here

			sce::Gnm::RenderTarget lightFBO; // FBO for our lighting pass
			PS4Texture* lightEmissiveTex; // emissive lighting
			PS4Texture* lightSpecularTex; // specular lighting

			PS4Shader* combineShader;
			PS4Shader* lightShader;
			PS4Shader* hudShader;
			PS4Mesh* lightSphere;
			PS4Mesh* screenQuad;

			Light* directionalLight;

			//GLuint hudTex;
			vector<HUDObject*> hudObjects;

			Vector4 ambientColour = Vector4(0.2f, 0.2f, 0.2f, 1.0f);

			int vertsDrawn = 0;
			int shadowCasters = 0;
			bool drawShadows = true;
		};
		#endif
	}
}

