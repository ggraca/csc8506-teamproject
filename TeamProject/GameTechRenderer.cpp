#include "GameTechRenderer.h"
#include "RenderObject.h"
#include "../Common/Camera.h"
#include "../Common/Vector2.h"
#include "../Common/Vector3.h"
#include "../Common/OBJGeometry.h"
#include "Light.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

#ifdef __ORBIS__
using namespace PS4;
#endif

#define SHADOWSIZE 4096

#ifdef _WIN32
GameTechRenderer::GameTechRenderer(GameWorld& world) : OGLRenderer(*Window::GetWindow()), gameWorld(world)	{
	shadowShader = new OGLShader("GameTechShadowVert.glsl", "GameTechShadowFrag.glsl");
	skyBoxShader = new OGLShader("skyboxVertex.glsl", "skyboxFragment.glsl");
	lightShader = new OGLShader("pointlightvert.glsl", "pointlightfrag.glsl");
	combineShader = new OGLShader("combinevert.glsl", "combinefrag.glsl");
	hudShader = new OGLShader("BasicVert.glsl", "BasicFrag.glsl");

	GenBuffers();

	screenQuad = OGLMesh::GenerateQuad();
	screenQuad->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	screenQuad->UploadToGPU();

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
	gBufferSpecularTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	//Generate light buffer textures
	lightEmissiveTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	lightSpecularTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);

	vector<TextureBase*> gBufferTexes;
	gBufferTexes.push_back(gBufferColourTex);
	gBufferTexes.push_back(gBufferNormalTex);
	gBufferTexes.push_back(gBufferSpecularTex);

	vector<TextureBase*> lightBufferTexes;
	lightBufferTexes.push_back(lightEmissiveTex);
	lightBufferTexes.push_back(lightSpecularTex);

	GenerateFrameBuffer(&gBufferFBO, gBufferTexes, gBufferDepthTex);
	GenerateFrameBuffer(&lightFBO, lightBufferTexes, nullptr);
}

void GameTechRenderer::RenderFrame() {
	pixOps.SetFaceCulling(CULLFACE::NOCULL);
	pixOps.SetDepthComparison(COMPARISON::LESS);
	pixOps.SetClearColor(Vector4(0.3f, 0.8f, 1, 1));
	BuildObjectList();
	SortObjectList();
	RenderShadowMap();
	RenderSkybox();
	RenderCamera();
	RenderLights();
	CombineBuffers();
	RenderHUD();
	pixOps.SetFaceCulling(CULLFACE::NOCULL); //Todo - text indices are going the wrong way...
}

void GameTechRenderer::BuildObjectList() {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	gameWorld.GetObjectIterators(first, last);

	activeObjects.clear();
	activeLights.clear();

	for (std::vector<GameObject*>::const_iterator i = first; i != last; ++i) {
		if ((*i)->IsActive()) {
			const RenderObject* g = (*i)->GetComponent<RenderObject*>();
			const Light* l = (*i)->GetComponent<Light*>();

			if (g) {
				activeObjects.emplace_back(g);
			}

			if (l) {
				activeLights.emplace_back(l);
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
		Vector3 directionalLightPos;
	for (int i = 0; i < activeLights.size(); i++)
	{
		if (activeLights[i]->GetGameObject()->GetName() == "Directional Light") {
			directionalLightPos = activeLights[i]->GetGameObject()->GetTransform().GetWorldPosition();
		}
	}

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(directionalLightPos, Vector3(0, 0, 0));
	Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 5000.0f, 1, 45.0f);
	Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;
	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on

	for (const auto&i : activeObjects) {
		Matrix4 modelMatrix = (*i).GetTransform()->GetWorldMatrix();
		Matrix4 mvpMatrix = mvMatrix * modelMatrix;
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
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl *>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl *>()->BuildProjectionMatrix(screenAspect);

	pixOps.SetDepthMask(false);
	pixOps.SetDepthComparison(COMPARISON::NOCOMPARE);
	pixOps.SetSourceFactor(BLEND::NOBLEND);
	pixOps.SetFaceCulling(CULLFACE::NOCULL);

	BindShader(skyBoxShader);

	BindTextureCubeToShader((OGLTexture*)skybox, "cubeTex", 2);
	BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
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
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);

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

		BindTextureCubeToShader((OGLTexture*)skybox, "cubeTex", 8);

		BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");//TODO give child position
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
	pixOps.SetClearColor(Vector4(0, 0, 0, 1));
	ClearBuffer(false, true, false);
	pixOps.SetSourceFactor(BLEND::ONE);
	pixOps.SetDestinationFactor(BLEND::ONE);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);

	BindShader(lightShader);

	Matrix4 identity;
	identity.ToIdentity();

	BindTextureToShader(gBufferDepthTex, "depthTex", 3);
	BindTextureToShader(gBufferNormalTex, "normTex", 4);


	for (int x = 0; x < activeLights.size(); ++x) {
		float radius = activeLights[x]->GetRadius();

		Matrix4 tempModelMatrix = Matrix4::Translation(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition())
			* activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation().ToMatrix4()
			* Matrix4::Scale(Vector3(radius, radius, radius));

		//Need to be able to bind vector2 to shader
		int pixelLocation = glGetUniformLocation(lightShader->GetProgramID(), "pixelSize");
		glUniform2f(pixelLocation, 1.0f / currentWidth, 1.0f / currentHeight);

		BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
		BindMatrix4ToShader(viewMatrix, "viewMatrix");
		BindMatrix4ToShader(projMatrix, "projMatrix");
		BindMatrix4ToShader(identity, "textureMatrix");
		BindVector3ToShader(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition(), "lightPos");
		BindVector4ToShader(activeLights[x]->GetColour(), "lightColour");
		BindFloatToShader(radius, "lightRadius");
		BindFloatToShader(activeLights[x]->GetBrightness(), "lightBrightness");
		BindMatrix4ToShader(tempModelMatrix, "modelMatrix");
		BindMatrix4ToShader(shadowMatrix, "shadowMatrix");

		if (activeLights[x]->GetGameObject()->GetName() == "Directional Light") {
			BindIntToShader(drawShadows, "drawShadows");
		}
		else {
			BindIntToShader(false, "drawShadows");
		}

		BindTextureToShader(shadowTex, "shadowTex", 20);

		float dist = (activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition() - gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition()).Length();

		if (activeLights[x]->GetType() == LightType::Point) {
			if (dist < radius) {// camera is inside the light volume !
				pixOps.SetFaceCulling(CULLFACE::FRONT);
			}
			else {
				pixOps.SetFaceCulling(CULLFACE::BACK);
			}

			BindMesh(lightSphere);
		}
		else if (activeLights[x]->GetType() == LightType::Spot) {
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

		//Calculates how many vertices are drawn per frame
		vertsDrawn += lightSphere->GetVertexCount();

		DrawBoundMesh();
	}

	pixOps.SetFaceCulling(CULLFACE::BACK);
	pixOps.SetSourceFactor(BLEND::SRC_ALPHA);
	pixOps.SetDestinationFactor(BLEND::ONE_MINUS_SRC_ALPHA);

	pixOps.SetClearColor(Vector4(0.2f, 0.2f, 0.2f, 1));

	BindFBO(nullptr);
	BindShader(nullptr);
}

void GameTechRenderer::CombineBuffers() {
	////Keep this here for now ready for when post processes are to be implemented
	//glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	//	GL_TEXTURE_2D, postProcessTex[1], 0);
	BindFBO(nullptr);
	ClearBuffer(true, true, false);

	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	Matrix4 tempProjMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	Matrix4 identity;
	identity.ToIdentity();

	BindShader(combineShader);

	BindMatrix4ToShader(identity, "modelMatrix");
	BindMatrix4ToShader(viewMatrix, "viewMatrix");
	BindMatrix4ToShader(tempProjMatrix, "projMatrix");
	BindMatrix4ToShader(identity, "textureMatrix");
	BindVector4ToShader(ambientColour, "ambientColour");

	BindTextureToShader(gBufferColourTex, "diffuseTex", 2);
	BindTextureToShader(gBufferDepthTex, "depthTex", 3);
	BindTextureToShader(gBufferNormalTex, "normTex", 4);
	BindTextureToShader(gBufferSpecularTex, "specularTex", 5);
	BindTextureToShader(lightEmissiveTex, "emissiveTex", 6);
	BindTextureToShader(lightSpecularTex, "lightSpecularTex", 7);

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
GameTechRenderer::GameTechRenderer(GameWorld& world) : PS4::PS4RendererBase((PS4::PS4Window*)Window::GetWindow()), gameWorld(world) {
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
	//ClearColor(Vector4(1, 1, 1, 1));
	////Set up the light properties
	//directionalLight = new Light(LightType::Point, Vector3(1000.0f, 1000.0f, 0.0f),
	//	Vector4(1.0f, 1.0f, 1.0f, 1.0f), 2000.0f, 3.0f, Quaternion(0, 0, 0, 0));
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
	//pixOps.SetClearColor(Vector4(0.3f, 0.8f, 1, 1));
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
//		Vector3 directionalLightPos;
//	for (int i = 0; i < activeLights.size(); i++)
//	{
//		if (activeLights[i]->GetGameObject()->GetName() == "Directional Light") {
//			directionalLightPos = activeLights[i]->GetGameObject()->GetTransform().GetWorldPosition();
//		}
//	}
//
//	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(directionalLightPos, Vector3(0, 0, 0));
//	Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 5000.0f, 1, 45.0f);
//	Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;
//	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on
//
//	for (const auto&i : activeObjects) {
//		Matrix4 modelMatrix = (*i).GetTransform()->GetWorldMatrix();
//		Matrix4 mvpMatrix = mvMatrix * modelMatrix;
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
//	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl *>()->BuildViewMatrix();
//	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl *>()->BuildProjectionMatrix(screenAspect);
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
	/*Matrix4*/ *viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	/*Matrix4*/ *projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);

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

		BindTextureCubeToShader((OGLTexture*)skybox, "cubeTex", 8);
		

		//BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
		*cameraPos = gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition();
		cameraPos = (Vector3*)onionAllocator->allocate(sizeof(Vector3), Gnm::kEmbeddedDataAlignment4);
		*cameraPos = Vector3();
		cameraBuffer.initAsConstantBuffer(cameraPos, sizeof(Vector3));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindMatrix4ToShader(projMatrix, "projMatrix");
		projMatrix = (Matrix4*)onionAllocator->allocate(sizeof(Matrix4), Gnm::kEmbeddedDataAlignment4);
		*projMatrix = Matrix4();
		cameraBuffer.initAsConstantBuffer(projMatrix, sizeof(Matrix4));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindMatrix4ToShader(viewMatrix, "viewMatrix");
		viewMatrix = (Matrix4*)onionAllocator->allocate(sizeof(Matrix4), Gnm::kEmbeddedDataAlignment4);
		*viewMatrix = Matrix4();
		cameraBuffer.initAsConstantBuffer(viewMatrix, sizeof(Matrix4));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindMatrix4ToShader((*i).GetTransform()->GetWorldMatrix(), "modelMatrix");
		*modelMatrix = (*i).GetTransform()->GetWorldMatrix();
		modelMatrix= (Matrix4*)onionAllocator->allocate(sizeof(Matrix4), Gnm::kEmbeddedDataAlignment4);
		*modelMatrix = Matrix4();
		cameraBuffer.initAsConstantBuffer(modelMatrix, sizeof(Matrix4));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindMatrix4ToShader(currentMaterial->GetTextureMatrix(), "textureMatrix");
		*textureMatrix = currentMaterial->GetTextureMatrix();
		textureMatrix = (Matrix4*)onionAllocator->allocate(sizeof(Matrix4), Gnm::kEmbeddedDataAlignment4);
		*textureMatrix = Matrix4();
		cameraBuffer.initAsConstantBuffer(textureMatrix, sizeof(Matrix4));
		cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);

		//BindVector4ToShader(currentMaterial->GetColour(), "objectColour");
		*objectColour = currentMaterial->GetColour();
		objectColour = (Vector4*)onionAllocator->allocate(sizeof(Vector3), Gnm::kEmbeddedDataAlignment4);
		*objectColour = Vector4();
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
//	pixOps.SetClearColor(Vector4(0, 0, 0, 1));
//	ClearBuffer(false, true, false);
//	pixOps.SetSourceFactor(BLEND::ONE);
//	pixOps.SetDestinationFactor(BLEND::ONE);
//
//	float screenAspect = (float)currentWidth / (float)currentHeight;
//	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
//	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);
//
//	BindShader(lightShader);
//
//	Matrix4 identity;
//	identity.ToIdentity();
//
//	BindTextureToShader(gBufferDepthTex, "depthTex", 3);
//	BindTextureToShader(gBufferNormalTex, "normTex", 4);
//
//
//	for (int x = 0; x < activeLights.size(); ++x) {
//		float radius = activeLights[x]->GetRadius();
//
//		Matrix4 tempModelMatrix = Matrix4::Translation(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition())
//			* activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation().ToMatrix4()
//			* Matrix4::Scale(Vector3(radius, radius, radius));
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
//	pixOps.SetClearColor(Vector4(0.2f, 0.2f, 0.2f, 1));
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
//	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
//	Matrix4 tempProjMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
//	Matrix4 identity;
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
