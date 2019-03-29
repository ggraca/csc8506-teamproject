#pragma once
#include "../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Common/TextureLoader.h"

#include "GameWorld.h"
#include "HUDObject.h"
#include "CameraControl.h"
#include "RenderObject.h"
#include "../Common/Frustum.h"

class Light;
class HUDObject;

namespace NCL {
	namespace CSC8503 {
		class RenderObject;
		class ParticleSystem;
		class PixOpsFlags;

		enum class DebugRenderState {
			STANDARD,
			DEPTH,
			NORMAL,
			DIFFUSE,
			MATERIAL,
			EMISSIVE,
			SPECULAR,
			SHADOW,
			MAX
		};

		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer();
			~GameTechRenderer();

			void SetGameWorld(GameWorld* gw) { gameWorld = gw; }
			int GetRendererWidth() const { return currentWidth; }
			int GetRendererHeight() const { return currentHeight; }
			int GetVertsDrawn() const { return vertsDrawn; }
			int GetShadowCasters() const { return shadowCasters; }

			void SetLightMesh(MeshGeometry* mesh) { lightSphere = mesh; }
			
			void SetSkyBox(TextureBase* Skybox) { skybox = Skybox; RegenerateIrradianceMap = true; }

			//HUD
			void HUDState(bool active) { activeHUD = active; }
			void AddHUDObjects();
			void WeaponState(int index, bool state);
			float health = 1; //(100%);
			//void UpdateHealthQuad();


		protected:
			void RenderFrame()	override;
			void GenBuffers();
			void RenderHUD();

			OGLShader*		defaultShader;

			GameWorld*	gameWorld;

			void BuildObjectList();
			void SortObjectList();
			vector<const RenderObject*>* FrustumCull(Frustum& frustum);
			void RenderShadowMap();
			void RenderSkybox();
			void RenderCamera();
			void RenderLights();
			void CombineBuffers();
			void RenderParticleSystems();
			void RenderPostProcess();
			void PresentScene();

			void SetupDebugMatrix(OGLShader*s) override;
			void RegisterRenderConsoleCommands();

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
			ShaderBase* convolutionShader;
			GLuint convFBO;
			TextureBase* skybox;
			TextureBase* irradianceMap;
			MeshGeometry* cube;
			bool RegenerateIrradianceMap = false;

			GLuint gBufferFBO; // FBO for our G- Buffer pass
			TextureBase* gBufferDepthTex; // Depth goes here
			TextureBase* gBufferColourTex; // Albedo goes here
			TextureBase* gBufferNormalTex; // Normals go here
			TextureBase* gBufferMaterialTex; // Specular goes here

			GLuint lightFBO; // FBO for our lighting pass
			TextureBase* lightEmissiveTex; // emissive lighting
			TextureBase* lightSpecularTex; // specular lighting
			TextureBase* lightKDTex;

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
			MeshGeometry* lightSphere;
			OGLMesh* screenQuad;

			GLuint hudTex;
			vector<HUDObject*> hudObjects;
			bool activeHUD = false;
			
			Vector4 ambientColour = Vector4(0.2f, 0.2f, 0.2f, 1.0f);

			int vertsDrawn = 0;
			int shadowCasters = 0;
			bool drawShadows = true;
			DebugRenderState activeRenderState = DebugRenderState::STANDARD;
		};
	}
}

