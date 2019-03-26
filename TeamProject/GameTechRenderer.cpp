#include "GameTechRenderer.h"
#include "RenderObject.h"
#include "../Common/Camera.h"
#include "../Common/Vector2.h"
#include "../Common/Vector3.h"
#include "../Common/OBJGeometry.h"
#include "Light.h"
#include "ParticleSystem.h"
#include "Debug.h"

#include "FunctionTimer.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

#ifdef __ORBIS__
using namespace PS4;
#endif

#define SHADOWSIZE 8192

#ifdef _WIN32
GameTechRenderer::GameTechRenderer() : OGLRenderer(*Window::GetWindow()) {
	shadowShader = Assets::AssetManager::LoadShader("GameTechShadowShader", "GameTechShadowVert.glsl", "GameTechShadowFrag.glsl");
	skyBoxShader = Assets::AssetManager::LoadShader("SkyboxShader", "skyboxVertex.glsl", "skyboxFragment.glsl");
	pointLightShader = Assets::AssetManager::LoadShader("PointLightShader", "pointlightvert.glsl", "pointlightfrag.glsl");
	directionalLightShader = Assets::AssetManager::LoadShader("DirectionalLightShader", "directionallightvert.glsl", "directionallightfrag.glsl");
	combineShader = Assets::AssetManager::LoadShader("CombineShader", "combinevert.glsl", "combinefrag.glsl");
	presentShader = Assets::AssetManager::LoadShader("PresentShader", "TexturedVertex.glsl", "TexturedFragment.glsl");
	particleShader = Assets::AssetManager::LoadShader("ParticleShader", "ParticleVert.glsl", "ParticleFrag.glsl");
	convolutionShader = Assets::AssetManager::LoadShader("ConvolutionShader", "convolutionVertex.glsl", "convolutionFragment.glsl");

	// Temporary until post process material or something is set up
	// postProcessShaders.push_back(Assets::AssetManager::LoadShader("PostShader", "TexturedVertex.glsl", "blurfrag.glsl"));

	hudShader = (OGLShader*)Assets::AssetManager::LoadShader("BasicShader", "BasicVert.glsl", "BasicFrag.glsl");

	GenBuffers();

	screenQuad = OGLMesh::GenerateQuad();
	screenQuad->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	screenQuad->UploadToGPU();
	cube = Assets::AssetManager::LoadMesh("Cube.msh");

	AddHUDObjects();

	pixOps.Init();
}

GameTechRenderer::~GameTechRenderer() {
	DeleteFrameBuffer(&gBufferFBO);
	DeleteFrameBuffer(&lightFBO);
	DeleteFrameBuffer(&shadowFBO);
}

void GameTechRenderer::AddHUDObjects()
{

	//Green HealthBar
	vector<OGLTexture*> textures1;
	textures1.push_back((OGLTexture*)TextureLoader::LoadAPITexture("healthBarGreen.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad(180.0f, 10.0f, (float)(currentHeight - 60.0f), (float)(currentHeight - 30.0f), currentWidth, currentHeight), textures1, Transform(), false));
	//Red HealthBar
	vector<OGLTexture*> textures2;
	textures2.push_back((OGLTexture*)TextureLoader::LoadAPITexture("healthBarRed.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad(180.0f, 10.0f, (float)(currentHeight - 60.0f), (float)(currentHeight - 30.0f), currentWidth, currentHeight), textures2, Transform(), false));
	//Hammer
	vector<OGLTexture*> textures3;
	textures3.push_back((OGLTexture*)TextureLoader::LoadAPITexture("hammer_gray.png"));
	textures3.push_back((OGLTexture*)TextureLoader::LoadAPITexture("hammer.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((currentWidth / 2) - 106.0f, (currentWidth / 2) - 42.0f, 20.0f, 84.0f, currentWidth, currentHeight), textures3, Transform(), false));
	//Gun
	vector<OGLTexture*> textures4;
	textures4.push_back((OGLTexture*)TextureLoader::LoadAPITexture("gun_gray.png"));
	textures4.push_back((OGLTexture*)TextureLoader::LoadAPITexture("gun.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((currentWidth / 2) - 32.0f, (currentWidth / 2) + 32.0f, 20.0f, 84.0f, currentWidth, currentHeight), textures4, Transform(), false));
	//Bomb
	vector<OGLTexture*> textures5;
	textures5.push_back((OGLTexture*)TextureLoader::LoadAPITexture("bomb_gray.png"));
	textures5.push_back((OGLTexture*)TextureLoader::LoadAPITexture("bomb.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((currentWidth / 2) + 42.0f, (currentWidth / 2) + 106.0f, 20.0f, 84.0f, currentWidth, currentHeight), textures5, Transform(), false));
	//Crosshair
	vector<OGLTexture*> textures6;
	textures6.push_back((OGLTexture*)TextureLoader::LoadAPITexture("crosshair.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((currentWidth / 2.0f) - 36.0f, (currentWidth / 2.0f) + 36.0f,
		(currentHeight / 2) - 36.0f, (currentHeight / 2) + 36.0f, currentWidth, currentHeight), textures6, Transform(), false));
}

void GameTechRenderer::GenBuffers() {
	shadowTex = OGLTexture::ShadowTexture(SHADOWSIZE, SHADOWSIZE);
	vector<TextureBase*> emptytextures;
	GenerateFrameBuffer(&shadowFBO, emptytextures, shadowTex);

	//Generate G-Buffer textures
	gBufferDepthTex = OGLTexture::EmptyTexture(currentWidth, currentHeight, true);
	gBufferColourTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	gBufferNormalTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	gBufferMaterialTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	//Generate light buffer textures
	lightEmissiveTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	lightSpecularTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	lightKDTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	//Generate post process buffer textures
	postTexture[0] = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	postTexture[1] = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	//Generate Convolution buffer textures
	TextureBase* tempConvTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	irradianceMap = OGLTexture::EmptyCubeTexture(32, 32);

	vector<TextureBase*> gBufferTexes;
	gBufferTexes.push_back(gBufferColourTex);
	gBufferTexes.push_back(gBufferNormalTex);
	gBufferTexes.push_back(gBufferMaterialTex);

	vector<TextureBase*> lightBufferTexes;
	lightBufferTexes.push_back(lightEmissiveTex);
	lightBufferTexes.push_back(lightSpecularTex);
	lightBufferTexes.push_back(lightKDTex);

	vector<TextureBase*> postBufferTexes;
	postBufferTexes.push_back(postTexture[0]);

	vector<TextureBase*> convBufferTexes;
	convBufferTexes.push_back(tempConvTex);

	GenerateFrameBuffer(&gBufferFBO, gBufferTexes, gBufferDepthTex);
	GenerateFrameBuffer(&lightFBO, lightBufferTexes, nullptr);
	GenerateFrameBuffer(&postFBO, postBufferTexes, nullptr);
	GenerateFrameBuffer(&convFBO, convBufferTexes, nullptr);
	delete tempConvTex;
}

void GameTechRenderer::RenderFrame() {
	FunctionTimer timer("Render Frame");

	pixOps.SetFaceCulling(CULLFACE::NOCULL);
	pixOps.SetDepthComparison(COMPARISON::LESS);
	pixOps.SetClearColor(Vec4(0.3f, 0.8f, 1, 1));
	BuildObjectList();
	SortObjectList();


	if (RegenerateIrradianceMap) {
		GenerateIrradianceMap(skybox, irradianceMap, convolutionShader, cube, (void*)&convFBO);
		RegenerateIrradianceMap = false;
	}

	RenderShadowMap();
	RenderSkybox();
	RenderCamera();
	RenderLights();
	CombineBuffers();
	RenderParticleSystems();
	RenderPostProcess();
	PresentScene();
	RenderHUD();
	pixOps.SetFaceCulling(CULLFACE::NOCULL); //Todo - text indices are going the wrong way...
}

void GameTechRenderer::BuildObjectList() {
	activeObjects.clear();
	activeLights.clear();
	activeParticleSystems.clear();

	for (auto go : gameWorld->GetGameObjectList()) {
		if (go->IsActive()) {
			const RenderObject* ro = go->GetComponent<RenderObject*>();
			const Light* l = go->GetComponent<Light*>();
			ParticleSystem* p = go->GetComponent<ParticleSystem*>();
			if (ro) {
				activeObjects.emplace_back(ro);
			}

			if (l) {
				activeLights.emplace_back(l);
			}

			if (p) {
				activeParticleSystems.emplace_back(p);
			}
		}
	}
}

void GameTechRenderer::SortObjectList() {

}

void GameTechRenderer::RenderShadowMap() {
	BindFBO((void*)&shadowFBO);
	ClearBuffer(true, false, false);
	SetViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	pixOps.SetColourMask(std::make_tuple(false, false, false, false));
	pixOps.SetFaceCulling(CULLFACE::FRONT);

	BindShader(shadowShader);

	//Temporary code to work out which light is the directional
	Vec3 cameraPosition = gameWorld->GetMainCamera()->GetTransform().GetWorldPosition();
	Quaternion lightRot;
	for (int i = 0; i < (int)activeLights.size(); i++)
	{
		if (activeLights[i]->GetType() == LightType::Directional) {
			lightRot = activeLights[i]->GetGameObject()->GetTransform().GetWorldOrientation();
		}
	}
	Vec3 newlightPos = (lightRot * Vec3(0, 0, 1)) * 1000.0f;
	Vec3 newlightPosUp = (lightRot * Vec3(0, 1, 0));

	Debug::DrawLine(Vec3(0, 0, 0), lightRot * Vec3(20, 0, 0), Vec4(1, 0, 0, 1));
	Debug::DrawLine(Vec3(0, 0, 0), lightRot * Vec3(0, 20, 0), Vec4(0, 1, 0, 1));
	Debug::DrawLine(Vec3(0, 0, 0), lightRot * Vec3(0, 0, -20), Vec4(0, 0, 1, 1));

	Mat4 shadowViewMatrix = Mat4::BuildViewMatrix(cameraPosition + newlightPos, cameraPosition, newlightPosUp);
	Mat4 shadowProjMatrix = Mat4::Orthographic(10, 10000, 1000, -1000, 1000, -1000);
	Mat4 mvMatrix = shadowProjMatrix * shadowViewMatrix;
	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on

	for (const auto&i : activeObjects) {
		Mat4 modelMatrix = (*i).GetTransform()->GetWorldMatrix();
		Mat4 mvpMatrix = mvMatrix * modelMatrix;
		BindMatrix4ToShader(mvpMatrix, "mvpMatrix");
		BindMesh((*i).GetMesh());
		DrawBoundMesh();
	}

	// Calculates how many shadow casting lights are currently being renderered
	shadowCasters++;

	SetViewport(0, 0, currentWidth, currentHeight);
	BindFBO(nullptr);

	pixOps.SetColourMask(std::make_tuple(true, true, true, true));
	pixOps.SetFaceCulling(CULLFACE::BACK);
}

void GameTechRenderer::RenderSkybox() {
	BindFBO((void*)&gBufferFBO);
	ClearBuffer(true, true, false);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Mat4 viewMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl *>()->BuildViewMatrix();
	Mat4 projMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl *>()->BuildProjectionMatrix(screenAspect);

	pixOps.SetDepthMask(false);
	pixOps.SetDepthComparison(COMPARISON::NOCOMPARE);
	pixOps.SetSourceFactor(BLEND::NOBLEND);
	pixOps.SetFaceCulling(CULLFACE::NOCULL);

	BindShader(skyBoxShader);

	BindTextureCubeToShader((OGLTexture*)skybox, "cubeTex", 2);
	BindVector3ToShader(gameWorld->GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
	BindMatrix4ToShader(viewMatrix, "viewMatrix");
	BindMatrix4ToShader(projMatrix, "projMatrix");

	BindMesh(screenQuad);
	DrawBoundMesh();

	pixOps.SetDepthMask(true);
	pixOps.SetDepthComparison(COMPARISON::LESS);
	pixOps.SetSourceFactor(BLEND::ONE);
	pixOps.SetFaceCulling(CULLFACE::BACK);

	BindShader(nullptr);
	BindFBO(nullptr);
}

void GameTechRenderer::RenderCamera() {
	BindFBO((void*)&gBufferFBO);
	ClearBuffer(true, false, false);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Mat4 viewMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	Mat4 projMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);

	shadowCasters = 0;
	vertsDrawn = 0;




	for (const auto&i : activeObjects) {

		Material* currentMaterial = (*i).GetMaterial();
		OGLShader* shader = (OGLShader*)currentMaterial->GetShader();
		BindShader(shader);

		for (unsigned int j = 0; j < currentMaterial->GetTextureParameters()->size(); j++)
		{
			//Gets texture, then gets texture shader param name, and binds it to texture slot = iteration
			BindTextureToShader((*(currentMaterial->GetTextureParameters()))[j].second,
				(*(currentMaterial->GetTextureParameters()))[j].first, j);
		}

		BindVector3ToShader(gameWorld->GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");//TODO give child position
		BindMatrix4ToShader(projMatrix, "projMatrix");
		BindMatrix4ToShader(viewMatrix, "viewMatrix");
		BindMatrix4ToShader((*i).GetTransform()->GetWorldMatrix(), "modelMatrix");
		BindMatrix4ToShader(currentMaterial->GetTextureMatrix(), "textureMatrix");
		BindVector4ToShader(currentMaterial->GetColour(), "objectColour");

		BindMesh((*i).GetMesh());
		vertsDrawn += (*i).GetMesh()->GetVertexCount();
		DrawBoundMesh();
	}

	BindFBO(nullptr);
}

void GameTechRenderer::RenderLights() {
	BindFBO((void*)&lightFBO);
	pixOps.SetClearColor(Vec4(0, 0, 0, 1));
	ClearBuffer(false, true, false);
	pixOps.SetSourceFactor(BLEND::ONE);
	pixOps.SetDestinationFactor(BLEND::ONE);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Mat4 viewMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	Mat4 projMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);

	BindShader(directionalLightShader);

	Mat4 identity;
	identity.ToIdentity();

	for (int x = 0; x < (int)activeLights.size(); ++x) {
		float radius = activeLights[x]->GetRadius();
		float dist = (activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition() -
			gameWorld->GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition()).Length();
		Mat4 tempModelMatrix;

		if (activeLights[x]->GetType() == LightType::Point) {
			BindShader(pointLightShader);

			tempModelMatrix = Mat4::Translation(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition())
				* activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation().ToMatrix4()
				* Mat4::Scale(Vec3(radius, radius, radius));

			BindTextureToShader(gBufferColourTex, "diffuseTex", 2);
			BindTextureToShader(gBufferDepthTex, "depthTex", 3);
			BindTextureToShader(gBufferNormalTex, "normTex", 4);
			BindTextureToShader(gBufferMaterialTex, "materialTex", 5);
			BindMatrix4ToShader(projMatrix, "projMatrix");
			BindMatrix4ToShader(tempModelMatrix, "modelMatrix");
			BindVector3ToShader(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition(), "lightPos");
			BindFloatToShader(radius, "lightRadius");
			//Turning shadows off until we can do point light shadows
			BindIntToShader(false, "drawShadows");
		
			if (dist < radius) {// camera is inside the light volume !
				pixOps.SetFaceCulling(CULLFACE::FRONT);
			}
			else {
				pixOps.SetFaceCulling(CULLFACE::BACK);
			}

			BindMesh(lightSphere);
		}
		else if (activeLights[x]->GetType() == LightType::Directional) {
			BindShader(directionalLightShader);

			Mat4 tempProjMatrix = Mat4::Orthographic(-1, 1, 1, -1, -1, 1);

			BindTextureToShader(gBufferColourTex, "diffuseTex", 2);
			BindTextureToShader(gBufferDepthTex, "depthTex", 3);
			BindTextureToShader(gBufferNormalTex, "normTex", 4);
			BindTextureToShader(gBufferMaterialTex, "materialTex", 5);
			BindTextureCubeToShader(skybox, "cubeTex", 6);
			BindMatrix4ToShader(tempProjMatrix, "projMatrix");
			BindMatrix4ToShader(projMatrix, "cameraProjMatrix");
			BindVector3ToShader(activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation() * Vec3(0, 0, -1), "lightDirection");
			BindIntToShader(drawShadows, "drawShadows");
			BindTextureToShader(shadowTex, "shadowTex", 20);
			BindMesh(screenQuad);
		}
		else if (activeLights[x]->GetType() == LightType::Spot) {
			//Spotlight shader when ready
			BindShader(pointLightShader);

			tempModelMatrix = Mat4::Translation(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition())
				* activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation().ToMatrix4()
				* Mat4::Scale(Vec3(radius, radius, radius));

			BindTextureToShader(gBufferColourTex, "diffuseTex", 2);
			BindTextureToShader(gBufferDepthTex, "depthTex", 3);
			BindTextureToShader(gBufferNormalTex, "normTex", 4);
			BindTextureToShader(gBufferMaterialTex, "materialTex", 5);
			BindMatrix4ToShader(projMatrix, "projMatrix");
			BindMatrix4ToShader(tempModelMatrix, "modelMatrix");
			BindVector3ToShader(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition(), "lightPos");
			BindFloatToShader(radius, "lightRadius");
			//Turning shadows off until we can do point light shadows
			BindIntToShader(false, "drawShadows");
			// Different calculation here to determine if inside the cone
			if (dist < radius) {// camera is inside the light volume !
				pixOps.SetFaceCulling(CULLFACE::FRONT);
			}
			else {
				pixOps.SetFaceCulling(CULLFACE::BACK);
			}

			//Change to bind cone.
			BindMesh(lightSphere);
		}

		BindVector2ToShader(Vec2(1.0f / currentWidth, 1.0f / currentHeight), "pixelSize");
		BindVector3ToShader(gameWorld->GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
		BindMatrix4ToShader(viewMatrix, "viewMatrix");
		BindMatrix4ToShader(identity, "textureMatrix");
		BindVector4ToShader(activeLights[x]->GetColour(), "lightColour");
		BindFloatToShader(activeLights[x]->GetBrightness(), "lightBrightness");
		BindMatrix4ToShader(shadowMatrix, "shadowMatrix");

		//Calculates how many vertices are drawn per frame
		vertsDrawn += lightSphere->GetVertexCount();

		DrawBoundMesh();
	}

	pixOps.SetFaceCulling(CULLFACE::BACK);
	pixOps.SetSourceFactor(BLEND::SRC_ALPHA);
	pixOps.SetDestinationFactor(BLEND::ONE_MINUS_SRC_ALPHA);

	pixOps.SetClearColor(Vec4(0.2f, 0.2f, 0.2f, 1));

	BindFBO(nullptr);
	BindShader(nullptr);
}

void GameTechRenderer::CombineBuffers() {
	//Keep this here for now ready for when post processes are to be implemented
	BindFBO((void*)&postFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, ((OGLTexture*)postTexture[0])->GetObjectID(), 0);
	ClearBuffer(true, true, false);

	Mat4 viewMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	Mat4 tempProjMatrix = Mat4::Orthographic(-1, 1, 1, -1, -1, 1);
	Mat4 identity;
	identity.ToIdentity();

	BindShader(combineShader);

	BindMatrix4ToShader(identity, "modelMatrix");
	BindMatrix4ToShader(viewMatrix, "viewMatrix");
	BindMatrix4ToShader(tempProjMatrix, "projMatrix");
	BindMatrix4ToShader(identity, "textureMatrix");

	BindTextureToShader(gBufferColourTex, "diffuseTex", 2);
	BindTextureToShader(gBufferMaterialTex, "materialTex", 3);
	BindTextureToShader(lightEmissiveTex, "emissiveTex", 4);
	BindTextureToShader(lightSpecularTex, "lightSpecularTex", 5);
	BindTextureToShader(lightKDTex, "KDTex", 6);
	BindTextureToShader(gBufferNormalTex, "normTex", 7);
	BindTextureCubeToShader(irradianceMap, "irradianceMap", 8);

	BindMesh(screenQuad);
	DrawBoundMesh();
	lastRendererdPostTex = 0;

	BindShader(nullptr);
	BindFBO(nullptr);
}

void GameTechRenderer::RenderParticleSystems() {
	BindFBO((void*)&postFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, ((OGLTexture*)postTexture[0])->GetObjectID(), 0);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Mat4 viewMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	Mat4 projMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);
	Mat4 identity;
	identity.ToIdentity();

	pixOps.SetFaceCulling(CULLFACE::NOCULL);
	pixOps.SetDepthMask(false);
	pixOps.SetSourceFactor(BLEND::ONE);
	pixOps.SetDestinationFactor(BLEND::ONE_MINUS_SRC_ALPHA);

	BindShader(particleShader);
	BindMesh(screenQuad);

	for (size_t i = 0; i < activeParticleSystems.size(); i++)
	{

		vector<Particle> particles = activeParticleSystems[i]->GetParticles();

		BindTextureToShader(activeParticleSystems[i]->GetParticleTexture(), "diffuseTex", 0);
		BindMatrix4ToShader(identity, "identity");

		//Adding string is also SLOW AS HELL
		for (size_t x = 0; x < particles.size(); x++)
		{
			Particle p = particles[x];
			BindVector4ToShader(Vec4(p.position.x, p.position.y, p.position.z, p.size),
				"worldPosition[" + std::to_string(x) + "]");
			BindFloatToShader(p.lifetime, "particleLifeTime[" + std::to_string(x) + "]");
		}
		BindMatrix4ToShader(viewMatrix, "viewMatrix");
		BindMatrix4ToShader(projMatrix, "projMatrix");
		BindFloatToShader(activeParticleSystems[i]->GetParticleMaxLifeTime(), "particleMaxLifeTime");

		//Temporary to test colour
		BindVector3ToShader(Vec3(0.2f, 0.2f, 0.2f), "objectColour");

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles.size());
	}

	pixOps.SetDestinationFactor(BLEND::ONE_MINUS_SRC_ALPHA);
	pixOps.SetSourceFactor(BLEND::SRC_ALPHA);
	pixOps.SetDepthMask(true);
	pixOps.SetFaceCulling(CULLFACE::BACK);

	BindShader(nullptr);
	BindFBO(nullptr);
}

void GameTechRenderer::RenderPostProcess() {
	BindFBO((void*)&postFBO);

	Mat4 tempProjMatrix = Mat4::Orthographic(-1, 1, 1, -1, -1, 1);

	DoPostProcess = false;
	if (DoPostProcess) {
		for (int i = 0; i < postProcessShaders.size(); i++)
		{
			int currentRendererdPostTex = (lastRendererdPostTex + 1) % 2;
			//Create bind texture to framebuffer color attachment
			//Like this:
			//							TextureBase*								Attachment slot
			//BindTextureToFBO(((OGLTexture*)postTexture[currentRendererdPostTex])->GetObjectID(), 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, ((OGLTexture*)postTexture[currentRendererdPostTex])->GetObjectID(), 0);
			ClearBuffer(true, true, false);

			BindShader(postProcessShaders[i]);

			//These lines are just setup for a blur shader
			//Should really be handled by a post process shader class (maybe material?)
			BindVector2ToShader(Vec2(1.0f / currentWidth, 1.0f / currentHeight), "pixelSize");
			BindMatrix4ToShader(tempProjMatrix, "projMatrix");
			for (int x = 0; x < 2; ++x) {
				currentRendererdPostTex = (lastRendererdPostTex + 1) % 2;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_2D, ((OGLTexture*)postTexture[currentRendererdPostTex])->GetObjectID(), 0);
				ClearBuffer(true, true, false);
				BindIntToShader(0, "isVertical");

				BindTextureToShader(postTexture[lastRendererdPostTex], "diffuseTex", 0);
				BindMesh(screenQuad);
				DrawBoundMesh();
				lastRendererdPostTex = currentRendererdPostTex;

				// Now to swap the colour buffers , and do the second blur pass
				currentRendererdPostTex = (lastRendererdPostTex + 1) % 2;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_2D, ((OGLTexture*)postTexture[currentRendererdPostTex])->GetObjectID(), 0);
				ClearBuffer(true, true, false);
				BindIntToShader(1, "isVertical");

				BindTextureToShader(postTexture[lastRendererdPostTex], "diffuseTex", 0);
				BindMesh(screenQuad);
				DrawBoundMesh();
				lastRendererdPostTex = currentRendererdPostTex;
			}

			lastRendererdPostTex = currentRendererdPostTex;
		}
	}
	BindFBO(nullptr);
	BindShader(nullptr);
}

void GameTechRenderer::PresentScene() {
	BindFBO(nullptr);
	pixOps.SetClearColor(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	ClearBuffer(true, true, false);
	BindShader(presentShader);

	Mat4 tempProjMatrix = Mat4::Orthographic(-1, 1, 1, -1, -1, 1);

	BindTextureToShader(postTexture[lastRendererdPostTex], "diffuseTex", 0);
	BindMatrix4ToShader(tempProjMatrix, "projMatrix");

	BindMesh(screenQuad);
	DrawBoundMesh();

	BindShader(nullptr);
}

void GameTechRenderer::RenderHUD()
{
	ClearBuffer(true, false, false);
	glDisable(GL_CULL_FACE);
	BindShader(hudShader);
	for (unsigned int i = 0; i < hudObjects.size(); i++)
	{
		if (i == 0)
		{
			glEnable(GL_SCISSOR_TEST);
			glScissor(GLint(10.0f + 170.0f * (1.0f - health)), 180, 655, 685);
		}

		glActiveTexture(GL_TEXTURE8);
		if (hudObjects[i]->GetTexture().size() == 1)
		{
			glBindTexture(GL_TEXTURE_2D, hudObjects[i]->GetTexture()[0]->GetObjectID());
		}
		else
		{
			if (!hudObjects[i]->IsWeaponActive())
			{
				glBindTexture(GL_TEXTURE_2D, hudObjects[i]->GetTexture()[0]->GetObjectID());
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, hudObjects[i]->GetTexture()[1]->GetObjectID());
			}
		}
		glUniform1i(glGetUniformLocation(hudObjects[i]->GetShader()->GetProgramID(), "basicTexture"), 8);
		BindMesh(hudObjects[i]->GetObjectMesh());
		DrawBoundMesh();
		if (i == 0) glDisable(GL_SCISSOR_TEST);
	}

	glEnable(GL_CULL_FACE);
}

void GameTechRenderer::WeaponState(int index, bool state)
{
	hudObjects[index]->SetWeaponState(state);
}

#endif

#ifdef __ORBIS__
GameTechRenderer::GameTechRenderer() : PS4::PS4RendererBase((PS4::PS4Window*)Window::GetWindow()){
	shadowShader = PS4Shader::GenerateShader("/app0/gameTechShadowVert.sb", "/app0/gameTechShadowPixel.sb");
	skyBoxShader = PS4Shader::GenerateShader("/app0/skyboxVertex.sb", "/app0/skyboxPixel.sb");
	lightShader = PS4Shader::GenerateShader("/app0/pointlightvert.sb", "/app0/pointlightPixel.sb");
	combineShader = PS4Shader::GenerateShader("/app0/combinevert.sb", "/app0/combinePixel.sb");
	hudShader = PS4Shader::GenerateShader("/app0/BasicVert.sb", "/app0/BasicPixel.sb");

	screenQuad = PS4Mesh::GenerateQuad();
	screenQuad->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	screenQuad->PS4Mesh::UploadToGPU();

	AddHUDObjects(); //adds hud textures
	skybox = PS4Texture::LoadTextureFromFile("/app0/cubeTex.gnf");
	Gnm::Sampler skyboxSampler;
	skyboxSampler.init();
	skyboxSampler.setXyFilterMode(Gnm::kFilterModeBilinear, Gnm::kFilterModeBilinear);
	skyboxSampler.setMipFilterMode(Gnm::kMipFilterModeLinear);

	//pixOps.Init();
	ClearBuffer(1, 1, 1);
	//ClearColor(Vec4(1, 1, 1, 1));
	////Set up the light properties
	//directionalLight = new Light(LightType::Point, Vec3(1000.0f, 1000.0f, 0.0f),
	//	Vec4(1.0f, 1.0f, 1.0f, 1.0f), 2000.0f, 3.0f, Quaternion(0, 0, 0, 0));
}

GameTechRenderer::~GameTechRenderer() {
	//DestroyVideoSystem();
}

void GameTechRenderer::AddHUDObjects()
{
	//Green HealthBar
	vector<PS4Texture*> textures1;
	textures1.push_back((PS4Texture*)PS4Texture::LoadTextureFromFile("/app0/healthBarGreen.gnf"));
	hudObjects.push_back(new HUDObject(PS4Mesh::GenerateQuad(180.0f, 10.0f, (float)(currentHeight - 60.0f), (float)(currentHeight - 30.0f), currentWidth, currentHeight), textures1, Transform(), false));
	//Red HealthBar
	vector<PS4Texture*> textures2;
	textures2.push_back((PS4Texture*)PS4Texture::LoadTextureFromFile("/app0/healthBarRed.gnf"));
	hudObjects.push_back(new HUDObject(PS4Mesh::GenerateQuad(180.0f, 10.0f, (float)(currentHeight - 60.0f), (float)(currentHeight - 30.0f), currentWidth, currentHeight), textures2, Transform(), false));
	//Hammer
	vector<PS4Texture*> textures3;
	textures3.push_back((PS4Texture*)PS4Texture::LoadTextureFromFile("/app0/hammer_gray.gnf"));
	textures3.push_back((PS4Texture*)PS4Texture::LoadTextureFromFile("/app0/hammer.gnf"));
	hudObjects.push_back(new HUDObject(PS4Mesh::GenerateQuad((currentWidth / 2) - 106.0f, (currentWidth / 2) - 42.0f, 20.0f, 84.0f, currentWidth, currentHeight), textures3, Transform(), false));
	//Gun
	vector<PS4Texture*> textures4;
	textures4.push_back((PS4Texture*)PS4Texture::LoadTextureFromFile("/app0/gun_gray.gnf"));
	textures4.push_back((PS4Texture*)PS4Texture::LoadTextureFromFile("/app0/gun.gnf"));
	hudObjects.push_back(new HUDObject(PS4Mesh::GenerateQuad((currentWidth / 2) - 32.0f, (currentWidth / 2) + 32.0f, 20.0f, 84.0f, currentWidth, currentHeight), textures4, Transform(), false));
	//Bomb
	vector<PS4Texture*> textures5;
	textures5.push_back((PS4Texture*)PS4Texture::LoadTextureFromFile("/app0/bomb_gray.gnf"));
	textures5.push_back((PS4Texture*)PS4Texture::LoadTextureFromFile("/app0/bomb.gnf"));
	hudObjects.push_back(new HUDObject(PS4Mesh::GenerateQuad((currentWidth / 2) + 42.0f, (currentWidth / 2) + 106.0f, 20.0f, 84.0f, currentWidth, currentHeight), textures5, Transform(), false));
	//Crosshair
	vector<PS4Texture*> textures6;
	textures6.push_back((PS4Texture*)PS4Texture::LoadTextureFromFile("/app0/crosshair.gnf"));
	hudObjects.push_back(new HUDObject(PS4Mesh::GenerateQuad((currentWidth / 2.0f) - 36.0f, (currentWidth / 2.0f) + 36.0f,
		(currentHeight / 2) - 36.0f, (currentHeight / 2) + 36.0f, currentWidth, currentHeight), textures6, Transform(), false));

	//may need to add samplers for each of these
}

//void GameTechRenderer::GenBuffers() {
//	shadowTex = PS4Texture::ShadowTexture(SHADOWSIZE, SHADOWSIZE);
//	vector<TextureBase*> emptytextures;
//	GenerateFrameBuffer(&shadowFBO, emptytextures, shadowTex);
//
//	//Generate G-Buffer textures
//	gBufferDepthTex = PS4Texture::EmptyTexture(currentWidth, currentHeight, true);
//	gBufferColourTex = PS4Texture::EmptyTexture(currentWidth, currentHeight);
//	gBufferNormalTex = PS4Texture::EmptyTexture(currentWidth, currentHeight);
//	gBufferSpecularTex = PS4Texture::EmptyTexture(currentWidth, currentHeight);
//	//Generate light buffer textures
//	lightEmissiveTex = PS4Texture::EmptyTexture(currentWidth, currentHeight);
//	lightSpecularTex = PS4Texture::EmptyTexture(currentWidth, currentHeight);
//
//	vector<TextureBase*> gBufferTexes;
//	gBufferTexes.push_back(gBufferColourTex);
//	gBufferTexes.push_back(gBufferNormalTex);
//	gBufferTexes.push_back(gBufferSpecularTex);
//
//	vector<TextureBase*> lightBufferTexes;
//	lightBufferTexes.push_back(lightEmissiveTex);
//	lightBufferTexes.push_back(lightSpecularTex);
//
//	GenerateFrameBuffer(&gBufferFBO, gBufferTexes, gBufferDepthTex);
//	GenerateFrameBuffer(&lightFBO, lightBufferTexes, nullptr);
//}

void GameTechRenderer::RenderFrame() {
	//pixOps.SetFaceCulling(CULLFACE::NOCULL);
	//pixOps.SetDepthComparison(COMPARISON::LESS);
	//pixOps.SetClearColor(Vec4(0.3f, 0.8f, 1, 1));
	//BuildObjectList();
	//SortObjectList();
	//RenderShadowMap();
	//RenderSkybox();
	RenderCamera();
	//RenderLights();
	//CombineBuffers();
	//RenderHUD();
	//pixOps.SetFaceCulling(CULLFACE::NOCULL); //Todo - text indices are going the wrong way...
}

void GameTechRenderer::BuildObjectList() {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	gameWorld.GetObjectIterators(first, last);

	activeObjects.clear();
	//activeLights.clear();

	for (std::vector<GameObject*>::const_iterator i = first; i != last; ++i) {
		if ((*i)->IsActive()) {
			const RenderObject* g = (*i)->GetComponent<RenderObject*>();
			const Light* l = (*i)->GetComponent<Light*>();

			if (g) {
				activeObjects.emplace_back(g);
			}

			if (l) {
				//activeLights.emplace_back(l);
			}
		}
	}
}

void GameTechRenderer::SortObjectList() {

}

//void GameTechRenderer::RenderShadowMap() {
//	BindFBO((void*)&shadowFBO);
//	ClearBuffer(true, false, false);
//	SetViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
//
//	pixOps.SetColourMask(std::make_tuple(false, false, false, false));
//	pixOps.SetFaceCulling(CULLFACE::FRONT);
//
//	BindShader(shadowShader);
//
//	Temporary code to work out which light is the directional
//		Vec3 directionalLightPos;
//	for (int i = 0; i < activeLights.size(); i++)
//	{
//		if (activeLights[i]->GetGameObject()->GetName() == "Directional Light") {
//			directionalLightPos = activeLights[i]->GetGameObject()->GetTransform().GetWorldPosition();
//		}
//	}
//
//	Mat4 shadowViewMatrix = Mat4::BuildViewMatrix(directionalLightPos, Vec3(0, 0, 0));
//	Mat4 shadowProjMatrix = Mat4::Perspective(100.0f, 5000.0f, 1, 45.0f);
//	Mat4 mvMatrix = shadowProjMatrix * shadowViewMatrix;
//	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on
//
//	for (const auto&i : activeObjects) {
//		Mat4 modelMatrix = (*i).GetTransform()->GetWorldMatrix();
//		Mat4 mvpMatrix = mvMatrix * modelMatrix;
//		BindMatrix4ToShader(mvpMatrix, "mvpMatrix");
//		BindMesh((*i).GetMesh());
//		DrawBoundMesh();
//	}
//
//	// Calculates how many shadow casting lights are currently being renderered
//	shadowCasters++;
//
//	SetViewport(0, 0, currentWidth, currentHeight);
//	BindFBO(nullptr);
//
//	pixOps.SetColourMask(std::make_tuple(true, true, true, true));
//	pixOps.SetFaceCulling(CULLFACE::BACK);
//}
//
//void GameTechRenderer::RenderSkybox() {
//	BindFBO((void*)&gBufferFBO);
//	ClearBuffer(true, true, false);
//
//	float screenAspect = (float)currentWidth / (float)currentHeight;
//	Mat4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl *>()->BuildViewMatrix();
//	Mat4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl *>()->BuildProjectionMatrix(screenAspect);
//
//	pixOps.SetDepthMask(false);
//	pixOps.SetDepthComparison(COMPARISON::NOCOMPARE);
//	pixOps.SetSourceFactor(BLEND::NOBLEND);
//	pixOps.SetFaceCulling(CULLFACE::NOCULL);
//
//	BindShader(skyBoxShader);
//
//	BindTextureCubeToShader((OGLTexture*)skybox, "cubeTex", 2);
//	BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
//	BindMatrix4ToShader(viewMatrix, "viewMatrix");
//	BindMatrix4ToShader(projMatrix, "projMatrix");
//
//	BindMesh(screenQuad);
//	DrawBoundMesh();
//
//	pixOps.SetDepthMask(true);
//	pixOps.SetDepthComparison(COMPARISON::LESS);
//	pixOps.SetSourceFactor(BLEND::ONE);
//	pixOps.SetFaceCulling(CULLFACE::BACK);
//
//	BindShader(nullptr);
//	BindFBO(nullptr);
//}

void GameTechRenderer::RenderCamera() {
	BindFBO((void*)&gBufferFBO);
	ClearBuffer(true, false, false);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	/*Mat4*/ *viewMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	/*Mat4*/ *projMatrix = gameWorld->GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);

	shadowCasters = 0;
	vertsDrawn = 0;

	Gnm::Sampler trilinearSampler;
	trilinearSampler.init();
	trilinearSampler.setMipFilterMode(Gnm::kMipFilterModeLinear);

	currentGFXContext->setTextures(Gnm::kShaderStagePs, 0, 1, &defaultTexture->GetAPITexture());
	currentGFXContext->setSamplers(Gnm::kShaderStagePs, 0, 1, &trilinearSampler);

	defaultShader->SubmitShaderSwitch(*currentGFXContext);
	currentGFXContext->setTextures(Gnm::kShaderStagePs, 0, 1, &defaultTexture->GetAPITexture());
	currentGFXContext->setSamplers(Gnm::kShaderStagePs, 0, 1, &trilinearSampler);

	for (const auto&i : activeObjects) {

		Material* currentMaterial = (*i).GetMaterial();
		PS4Shader* shader = (PS4Shader*)currentMaterial->GetShader();
		shader->SubmitShaderSwitch(*currentGFXContext);

		for (unsigned int j = 0; j < currentMaterial->GetTextureParameters()->size(); j++)
		{
			//Gets texture, then gets texture shader param name, and binds it to texture slot = iteration
			//BindTextureToShader((*(currentMaterial->GetTextureParameters()))[j].second, (*(currentMaterial->GetTextureParameters()))[j].first, j);
		}

		//BindTextureCubeToShader((OGLTexture*)skybox, "cubeTex", 8);
		

		//BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
		*cameraPos = gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition();
		cameraPos = (Vec3*)onionAllocator->allocate(sizeof(Vec3), Gnm::kEmbeddedDataAlignment4);
		*cameraPos = Vec3();
		cameraBuffer.initAsConstantBuffer(cameraPos, sizeof(Vec3));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindMatrix4ToShader(projMatrix, "projMatrix");
		projMatrix = (Mat4*)onionAllocator->allocate(sizeof(Mat4), Gnm::kEmbeddedDataAlignment4);
		*projMatrix = Mat4();
		cameraBuffer.initAsConstantBuffer(projMatrix, sizeof(Mat4));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindMatrix4ToShader(viewMatrix, "viewMatrix");
		viewMatrix = (Mat4*)onionAllocator->allocate(sizeof(Mat4), Gnm::kEmbeddedDataAlignment4);
		*viewMatrix = Mat4();
		cameraBuffer.initAsConstantBuffer(viewMatrix, sizeof(Mat4));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindMatrix4ToShader((*i).GetTransform()->GetWorldMatrix(), "modelMatrix");
		*modelMatrix = (*i).GetTransform()->GetWorldMatrix();
		modelMatrix= (Mat4*)onionAllocator->allocate(sizeof(Mat4), Gnm::kEmbeddedDataAlignment4);
		*modelMatrix = Mat4();
		cameraBuffer.initAsConstantBuffer(modelMatrix, sizeof(Mat4));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindMatrix4ToShader(currentMaterial->GetTextureMatrix(), "textureMatrix");
		*textureMatrix = currentMaterial->GetTextureMatrix();
		textureMatrix = (Mat4*)onionAllocator->allocate(sizeof(Mat4), Gnm::kEmbeddedDataAlignment4);
		*textureMatrix = Mat4();
		cameraBuffer.initAsConstantBuffer(textureMatrix, sizeof(Mat4));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindVector4ToShader(currentMaterial->GetColour(), "objectColour");
		*objectColour = currentMaterial->GetColour();
		objectColour = (Vec4*)onionAllocator->allocate(sizeof(Vector3), Gnm::kEmbeddedDataAlignment4);
		*objectColour = Vec4();
		cameraBuffer.initAsConstantBuffer(objectColour, sizeof(Vector3));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindMesh((*i).GetMesh());
		(*i).GetMesh()->UploadToGPU();
		vertsDrawn += (*i).GetMesh()->GetVertexCount();
		DrawMesh((*i).GetMesh());
	}

	BindFBO(nullptr);
}

//void GameTechRenderer::RenderLights() {
//	BindFBO((void*)&lightFBO);
//	pixOps.SetClearColor(Vec4(0, 0, 0, 1));
//	ClearBuffer(false, true, false);
//	pixOps.SetSourceFactor(BLEND::ONE);
//	pixOps.SetDestinationFactor(BLEND::ONE);
//
//	float screenAspect = (float)currentWidth / (float)currentHeight;
//	Mat4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
//	Mat4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);
//
//	BindShader(lightShader);
//
//	Mat4 identity;
//	identity.ToIdentity();
//
//	BindTextureToShader(gBufferDepthTex, "depthTex", 3);
//	BindTextureToShader(gBufferNormalTex, "normTex", 4);
//
//
//	for (int x = 0; x < activeLights.size(); ++x) {
//		float radius = activeLights[x]->GetRadius();
//
//		Mat4 tempModelMatrix = Mat4::Translation(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition())
//			* activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation().ToMatrix4()
//			* Mat4::Scale(Vec3(radius, radius, radius));
//
//		//Need to be able to bind vector2 to shader
//		int pixelLocation = glGetUniformLocation(lightShader->GetProgramID(), "pixelSize");
//		glUniform2f(pixelLocation, 1.0f / currentWidth, 1.0f / currentHeight);
//
//		BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
//		BindMatrix4ToShader(viewMatrix, "viewMatrix");
//		BindMatrix4ToShader(projMatrix, "projMatrix");
//		BindMatrix4ToShader(identity, "textureMatrix");
//		BindVector3ToShader(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition(), "lightPos");
//		BindVector4ToShader(activeLights[x]->GetColour(), "lightColour");
//		BindFloatToShader(radius, "lightRadius");
//		BindFloatToShader(activeLights[x]->GetBrightness(), "lightBrightness");
//		BindMatrix4ToShader(tempModelMatrix, "modelMatrix");
//		BindMatrix4ToShader(shadowMatrix, "shadowMatrix");
//
//		if (activeLights[x]->GetGameObject()->GetName() == "Directional Light") {
//			BindIntToShader(drawShadows, "drawShadows");
//		}
//		else {
//			BindIntToShader(false, "drawShadows");
//		}
//
//		BindTextureToShader(shadowTex, "shadowTex", 20);
//
//		float dist = (activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition() - gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition()).Length();
//
//		if (activeLights[x]->GetType() == LightType::Point) {
//			if (dist < radius) {// camera is inside the light volume !
//				pixOps.SetFaceCulling(CULLFACE::FRONT);
//			}
//			else {
//				pixOps.SetFaceCulling(CULLFACE::BACK);
//			}
//
//			BindMesh(lightSphere);
//		}
//		else if (activeLights[x]->GetType() == LightType::Spot) {
//			// Different calculation here to determine if inside the cone
//			if (dist < radius) {// camera is inside the light volume !
//				pixOps.SetFaceCulling(CULLFACE::FRONT);
//			}
//			else {
//				pixOps.SetFaceCulling(CULLFACE::BACK);
//			}
//
//			//Change to bind cone.
//			BindMesh(lightSphere);
//		}
//
//		//Calculates how many vertices are drawn per frame
//		vertsDrawn += lightSphere->GetVertexCount();
//
//		DrawBoundMesh();
//	}
//
//	pixOps.SetFaceCulling(CULLFACE::BACK);
//	pixOps.SetSourceFactor(BLEND::SRC_ALPHA);
//	pixOps.SetDestinationFactor(BLEND::ONE_MINUS_SRC_ALPHA);
//
//	pixOps.SetClearColor(Vec4(0.2f, 0.2f, 0.2f, 1));
//
//	BindFBO(nullptr);
//	BindShader(nullptr);
//}
//
//void GameTechRenderer::CombineBuffers() {
//	////Keep this here for now ready for when post processes are to be implemented
//	//glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);
//	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//	//	GL_TEXTURE_2D, postProcessTex[1], 0);
//	BindFBO(nullptr);
//	ClearBuffer(true, true, false);
//
//	Mat4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
//	Mat4 tempProjMatrix = Mat4::Orthographic(-1, 1, 1, -1, -1, 1);
//	Mat4 identity;
//	identity.ToIdentity();
//
//	BindShader(combineShader);
//
//	BindMatrix4ToShader(identity, "modelMatrix");
//	BindMatrix4ToShader(viewMatrix, "viewMatrix");
//	BindMatrix4ToShader(tempProjMatrix, "projMatrix");
//	BindMatrix4ToShader(identity, "textureMatrix");
//	BindVector4ToShader(ambientColour, "ambientColour");
//
//	BindTextureToShader(gBufferColourTex, "diffuseTex", 2);
//	BindTextureToShader(gBufferDepthTex, "depthTex", 3);
//	BindTextureToShader(gBufferNormalTex, "normTex", 4);
//	BindTextureToShader(gBufferSpecularTex, "specularTex", 5);
//	BindTextureToShader(lightEmissiveTex, "emissiveTex", 6);
//	BindTextureToShader(lightSpecularTex, "lightSpecularTex", 7);
//
//	BindMesh(screenQuad);
//	DrawBoundMesh();
//
//	BindShader(nullptr);
//}
//
//void GameTechRenderer::RenderHUD()
//{
//	ClearBuffer(true, false, false);
//	glDisable(GL_CULL_FACE);
//	BindShader(hudShader);
//	for (unsigned int i = 0; i < hudObjects.size(); i++)
//	{
//		if (i == 0)
//		{
//			glEnable(GL_SCISSOR_TEST);
//			glScissor(GLint(10.0f + 170.0f * (1.0f - health)), 180, 655, 685);
//		}
//
//		glActiveTexture(GL_TEXTURE8);
//		if (hudObjects[i]->GetTexture().size() == 1)
//		{
//			glBindTexture(GL_TEXTURE_2D, hudObjects[i]->GetTexture()[0]->GetObjectID());
//		}
//		else
//		{
//			if (!hudObjects[i]->IsWeaponActive())
//			{
//				glBindTexture(GL_TEXTURE_2D, hudObjects[i]->GetTexture()[0]->GetObjectID());
//			}
//			else
//			{
//				glBindTexture(GL_TEXTURE_2D, hudObjects[i]->GetTexture()[1]->GetObjectID());
//			}
//		}
//		glUniform1i(glGetUniformLocation(hudObjects[i]->GetShader()->GetProgramID(), "basicTexture"), 8);
//		BindMesh(hudObjects[i]->GetObjectMesh());
//		DrawBoundMesh();
//		if (i == 0) glDisable(GL_SCISSOR_TEST);
//	}
//
//	glEnable(GL_CULL_FACE);
//}
//
//void GameTechRenderer::WeaponState(int index, bool state)
//{
//	hudObjects[index]->SetWeaponState(state);
//}
//
#endif


/*void GameTechRenderer::UpdateHealthQuad()
{
	delete healthBarQuadGreen;
	if (health < 0.01) healthBarQuadGreen = OGLMesh::GenerateQuad((180 * health) + 10, 10, 655, 685);
	else healthBarQuadGreen = OGLMesh::GenerateQuad((180 * health), 10, 655, 685);
	healthBarQuadGreen->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	healthBarQuadGreen->UploadToGPU();
}*/
