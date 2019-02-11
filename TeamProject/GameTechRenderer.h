#pragma once
#include "../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"

#include "../GameTechCommon/GameWorld.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
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

			void GenerateScreenTexture(GLuint & into, bool depth = false);

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

			void SetupDebugMatrix(OGLShader*s) override;

			vector<const RenderObject*> activeObjects;

			//shadow mapping things
			OGLShader*	shadowShader;
			GLuint		shadowTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			OGLShader* skyBoxShader;
			//Skybox needs to be loaded into the texture library
			GLuint skybox;

			GLuint gBufferFBO; // FBO for our G- Buffer pass
			GLuint gBufferDepthTex; // Depth goes here
			GLuint gBufferColourTex; // Albedo goes here
			GLuint gBufferNormalTex; // Normals go here
			GLuint gBufferSpecularTex; // Specular goes here

			GLuint lightFBO; // FBO for our lighting pass
			GLuint lightEmissiveTex; // emissive lighting
			GLuint lightSpecularTex; // specular lighting

			Vector4		lightColour;
			float		lightRadius;
			Vector3		lightPosition;

			int vertsDrawn = 0;
			int shadowCasters = 0;
		};
	}
}

