#pragma once
#include "../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Common/TextureLoader.h"

#include "GameWorld.h"

class Light;

namespace NCL {
	namespace CSC8503 {
		class RenderObject;
		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer(GameWorld& world);
			~GameTechRenderer();

			int GetRendererWidth() const { return currentWidth; }
			int GetRendererHeight() const { return currentHeight; }
			int GetVertsDrawn() const { return vertsDrawn; }
			int GetShadowCasters() const { return shadowCasters; }

			void SetLightMesh(OGLMesh* mesh) { lightSphere = mesh; }

			void GenerateScreenTexture(GLuint& into, bool depth = false);
			
			GLuint skybox;

			//HUD
			GLuint healthBarGreen;
			GLuint healthBarRed;
			float health = 1; //(100%);
			void UpdateHealthQuad();

		protected:
			void RenderFrame()	override;
			void GenBuffers();

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

			//shadow mapping things
			OGLShader*	shadowShader;
			GLuint		shadowTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			OGLShader* skyBoxShader;

			GLuint gBufferFBO; // FBO for our G- Buffer pass
			GLuint gBufferDepthTex; // Depth goes here
			GLuint gBufferColourTex; // Albedo goes here
			GLuint gBufferNormalTex; // Normals go here
			GLuint gBufferSpecularTex; // Specular goes here
			GLuint hudTex;

			GLuint lightFBO; // FBO for our lighting pass
			GLuint lightEmissiveTex; // emissive lighting
			GLuint lightSpecularTex; // specular lighting

			OGLShader* combineShader;
			OGLShader* lightShader;
			OGLShader* basicShader;
			OGLMesh* lightSphere;
			OGLMesh* screenQuad;
			OGLMesh* healthBarQuadGreen;
			OGLMesh* healthBarQuadRed;

			Light* directionalLight;
			
			
			Vector4 ambientColour = Vector4(0.2f, 0.2f, 0.2f, 1.0f);

			int vertsDrawn = 0;
			int shadowCasters = 0;

		};
	}
}

