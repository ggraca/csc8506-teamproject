#pragma once
#include "../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Common/TextureLoader.h"

#include "GameWorld.h"
#include "HUDObject.h"
#include "CameraControl.h"

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

			//shadow mapping things
			OGLShader*	shadowShader;
			GLuint		shadowTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			OGLShader* skyBoxShader;

			GLuint gBufferFBO; // FBO for our G- Buffer pass
			TextureBase* gBufferDepthTex; // Depth goes here
			TextureBase* gBufferColourTex; // Albedo goes here
			TextureBase* gBufferNormalTex; // Normals go here
			TextureBase* gBufferSpecularTex; // Specular goes here

			GLuint lightFBO; // FBO for our lighting pass
			TextureBase* lightEmissiveTex; // emissive lighting
			TextureBase* lightSpecularTex; // specular lighting

			OGLShader* combineShader;
			OGLShader* lightShader;
			OGLShader* hudShader;
			OGLMesh* lightSphere;
			OGLMesh* screenQuad;

			Light* directionalLight;
			
			GLuint hudTex;
			vector<HUDObject*> hudObjects;
			
			Vector4 ambientColour = Vector4(0.2f, 0.2f, 0.2f, 1.0f);

			int vertsDrawn = 0;
			int shadowCasters = 0;
			bool drawShadows = true;
		};

		#ifdef WIN32
		class PixOpsFlags {
		public:
			enum CULLFACE {
				NONE = 0,
				FRONT = 1,
				BACK = 2
			};

			void SetFaceCulling(CULLFACE cull) { 
				switch (cull) {
				case NONE:
					if (Culling != CULLFACE::NONE) {
						Culling = CULLFACE::NONE;
						glDisable(GL_CULL_FACE);
					}
					break;
				case FRONT:
					if (Culling == CULLFACE::NONE) {
						glEnable(GL_CULL_FACE);
					}

					if (Culling != CULLFACE::FRONT) {
						Culling = CULLFACE::FRONT;
						glCullFace(GL_FRONT);
					}
					break;
				case BACK:
					if (Culling == CULLFACE::NONE) {
						glEnable(GL_CULL_FACE);
					}

					if (Culling != CULLFACE::BACK) {
						Culling = CULLFACE::BACK;
						glCullFace(GL_BACK);
					}
					break;
				default:
					std::cout << "Set Face Culling with undefined mode" << endl;
					break;
				}
			};

			CULLFACE GetFaceCulling() { return Culling; }

		protected:
			CULLFACE Culling;

		private:


		};
		#endif

		#ifdef PS4
		class PixOpsFlags {
		public:
			enum CULLFACE {
				NONE = 0,
				FRONT = 1,
				BACK = 2
			};

			void EnableFaceCulling() { cullingEnabled = true; GNMEnable(GNM_CULL_FACE); }
			void DisableFaceCulling() { cullingEnabled = false; GNMDisable(GNM_CULL_FACE); }

		protected:
			bool cullingEnabled = false;
			bool cullingFront = false;

		private:


		};
		#endif
	}
}

