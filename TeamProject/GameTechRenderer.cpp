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

#define SHADOWSIZE 4096

Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5, 0.5, 0.5)) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));

GameTechRenderer::GameTechRenderer(GameWorld& world) : OGLRenderer(*Window::GetWindow()), gameWorld(world)	{
	shadowShader = Assets::AssetManager::LoadShader("GameTechShadowShader", "GameTechShadowVert.glsl", "GameTechShadowFrag.glsl");
	skyBoxShader = Assets::AssetManager::LoadShader("SkyboxShader", "skyboxVertex.glsl", "skyboxFragment.glsl");
	pointLightShader = Assets::AssetManager::LoadShader("PointLightShader", "pointlightvert.glsl", "pointlightfrag.glsl");
	directionalLightShader = Assets::AssetManager::LoadShader("DirectionalLightShader", "directionallightvert.glsl", "directionallightfrag.glsl");
	combineShader = Assets::AssetManager::LoadShader("CombineShader", "combinevert.glsl", "combinefrag.glsl");


	hudShader = (OGLShader*)Assets::AssetManager::LoadShader("BasicShader", "BasicVert.glsl", "BasicFrag.glsl");

	GenBuffers();

	screenQuad = OGLMesh::GenerateQuad();
	screenQuad->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	screenQuad->UploadToGPU();

	AddHUDObjects();

	pixOps.Init();
}


GameTechRenderer::~GameTechRenderer()	{
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
	//Generate post process buffer textures
	postTexture[0] = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	postTexture[1] = OGLTexture::EmptyTexture(currentWidth, currentHeight);

	vector<TextureBase*> gBufferTexes;
	gBufferTexes.push_back(gBufferColourTex);
	gBufferTexes.push_back(gBufferNormalTex);
	gBufferTexes.push_back(gBufferSpecularTex);

	vector<TextureBase*> lightBufferTexes;
	lightBufferTexes.push_back(lightEmissiveTex);
	lightBufferTexes.push_back(lightSpecularTex);

	vector<TextureBase*> postBufferTexes;
	postBufferTexes.push_back(postTexture[0]);

	GenerateFrameBuffer(&gBufferFBO, gBufferTexes, gBufferDepthTex);
	GenerateFrameBuffer(&lightFBO, lightBufferTexes, nullptr);
	GenerateFrameBuffer(&postFBO, postBufferTexes, nullptr);
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
	Vector3 cameraPosition = gameWorld.GetMainCamera()->GetTransform().GetWorldPosition();
	Quaternion lightRot;
	for (int i = 0; i < (int)activeLights.size(); i++)
	{
		if (activeLights[i]->GetType() == LightType::Directional) {
			lightRot = activeLights[i]->GetGameObject()->GetTransform().GetWorldOrientation();
		}
	}
	Vector3 newlightPos = (lightRot * Vector3(0, 0, 1)) * 1000.0f;
	Vector3 newlightPosUp = (lightRot * Vector3(0, 1, 0));

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(cameraPosition + newlightPos, cameraPosition, newlightPosUp);
	Matrix4 shadowProjMatrix = Matrix4::Orthographic(-10, 10000, -1000, 1000, -1000, 1000);
	Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;
	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on

	for (const auto&i : activeObjects) {
		Matrix4 modelMatrix = (*i).GetTransform()->GetWorldMatrix();
		Matrix4 mvpMatrix	= mvMatrix * modelMatrix;
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

	BindShader(directionalLightShader);

	Matrix4 identity;
	identity.ToIdentity();

	for (int x = 0; x < (int)activeLights.size(); ++x) {
		float radius = activeLights[x]->GetRadius();
		float dist = (activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition() -
			gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition()).Length();
		Matrix4 tempModelMatrix;

		if (activeLights[x]->GetType() == LightType::Point) {
			BindShader(pointLightShader);

			tempModelMatrix = Matrix4::Translation(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition())
				* activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation().ToMatrix4()
				* Matrix4::Scale(Vector3(radius, radius, radius));

			BindTextureToShader(gBufferDepthTex, "depthTex", 3);
			BindTextureToShader(gBufferNormalTex, "normTex", 4);
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

			Matrix4 tempProjMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

			BindTextureToShader(gBufferDepthTex, "depthTex", 3);
			BindTextureToShader(gBufferNormalTex, "normTex", 4);
			BindMatrix4ToShader(tempProjMatrix, "projMatrix");
			BindMatrix4ToShader(projMatrix, "cameraProjMatrix");
			Vector3 rot = activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation() * Vector3(0, 0, 1);
			BindVector3ToShader(activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation() * Vector3(0, 0, 1), "lightDir");
			BindIntToShader(drawShadows, "drawShadows");
			BindTextureToShader(shadowTex, "shadowTex", 20);
			BindMesh(screenQuad);
		}
		else if (activeLights[x]->GetType() == LightType::Spot) {
			//Spotlight shader when ready
			BindShader(pointLightShader);

			tempModelMatrix = Matrix4::Translation(activeLights[x]->GetGameObject()->GetTransform().GetWorldPosition())
				* activeLights[x]->GetGameObject()->GetTransform().GetWorldOrientation().ToMatrix4()
				* Matrix4::Scale(Vector3(radius, radius, radius));

			BindTextureToShader(gBufferDepthTex, "depthTex", 3);
			BindTextureToShader(gBufferNormalTex, "normTex", 4);
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

		BindVector2ToShader(Vector2(1.0f / currentWidth, 1.0f / currentHeight), "pixelSize");
		BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
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

	pixOps.SetClearColor(Vector4(0.2f, 0.2f, 0.2f, 1));

	BindFBO(nullptr);
	BindShader(nullptr);
}

void GameTechRenderer::CombineBuffers() {
	//Keep this here for now ready for when post processes are to be implemented
	BindFBO((void*)&postFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, ((OGLTexture*)postTexture[0])->GetObjectID(), 0);
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
	BindFBO(nullptr);
}

void GameTechRenderer::RenderPostProcess() {
	BindFBO((void*)&postFBO);
	for (int i = 0; i < postProcessShaders.size(); i++)
	{
		int currentRendererdPostTex = (i + 1) % 2;
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
		BindVector2ToShader(Vector2(1.0f / currentWidth, 1.0f / currentHeight), "pixelSize");
		for (int x = 0; x < 10; ++x) {
			currentRendererdPostTex = (i + x + 1) % 2;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, ((OGLTexture*)postTexture[currentRendererdPostTex])->GetObjectID(), 0);
			BindIntToShader(0, "isVertical");

			BindTextureToShader(postTexture[lastRendererdPostTex], "diffuseTex", 0);
			BindMesh(screenQuad);
			DrawBoundMesh();
			lastRendererdPostTex = currentRendererdPostTex;

			// Now to swap the colour buffers , and do the second blur pass
			currentRendererdPostTex = (lastRendererdPostTex + 1) % 2;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, ((OGLTexture*)postTexture[currentRendererdPostTex])->GetObjectID(), 0);
			BindIntToShader(1, "isVertical");

			BindTextureToShader(postTexture[lastRendererdPostTex], "diffuseTex", 0);
			BindMesh(screenQuad);
			DrawBoundMesh();
			lastRendererdPostTex = currentRendererdPostTex;
		}

		lastRendererdPostTex = currentRendererdPostTex;
	}
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

void GameTechRenderer::SetupDebugMatrix(OGLShader*s) {
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);

	Matrix4 vp = projMatrix * viewMatrix;

	int matLocation = glGetUniformLocation(s->GetProgramID(), "viewProjMatrix");

	glUniformMatrix4fv(matLocation, 1, false, (float*)&vp);
}



/*void GameTechRenderer::UpdateHealthQuad()
{
	delete healthBarQuadGreen;
	if (health < 0.01) healthBarQuadGreen = OGLMesh::GenerateQuad((180 * health) + 10, 10, 655, 685);
	else healthBarQuadGreen = OGLMesh::GenerateQuad((180 * health), 10, 655, 685);
	healthBarQuadGreen->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	healthBarQuadGreen->UploadToGPU();
}*/
