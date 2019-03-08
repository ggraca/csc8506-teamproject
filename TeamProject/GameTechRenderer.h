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

class Light;
class HUDObject;

namespace NCL {
	namespace CSC8503 {
		class RenderObject;
		class PixOpsFlags;
		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer(GameWorld& world);
			~GameTechRenderer();

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


		protected:
			void RenderFrame()	override;
			void GenBuffers();
			void RenderHUD();

			OGLShader*		defaultShader;

			GameWorld&	gameWorld;

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderSkybox();
			void RenderCamera();
			void RenderLights();
			void CombineBuffers();

			void SetupDebugMatrix(OGLShader*s) override;

			vector<const RenderObject*> activeObjects;
			vector<const Light*> activeLights;

			//shadow mapping things
			ShaderBase*	shadowShader;
			GLuint		shadowTex;
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

			ShaderBase* combineShader;
			ShaderBase* lightShader;
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
	}
}

