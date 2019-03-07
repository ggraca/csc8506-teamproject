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

	ClearColor(Vector4(1, 1, 1, 1));
	//Set up the light properties
	directionalLight = new Light(LightType::Point, Vector3(1000.0f, 1000.0f, 0.0f),
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), 2000.0f, 3.0f, Quaternion(0, 0, 0, 0));
}


GameTechRenderer::~GameTechRenderer()	{
	DeleteFrameBuffer(&gBufferFBO);
	DeleteFrameBuffer(&lightFBO);
	DeleteFrameBuffer(&shadowFBO);

	delete directionalLight;
}

void GameTechRenderer::AddHUDObjects()
{

	//Green HealthBar
	vector<OGLTexture*> textures1;
	textures1.push_back((OGLTexture*)Assets::AssetManager::LoadTexture("healthBarGreen.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad(180, 10, 655, 685), textures1, Transform(), false));
	//Red HealthBar
	vector<OGLTexture*> textures2;
	textures2.push_back((OGLTexture*)Assets::AssetManager::LoadTexture("healthBarRed.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad(180, 10, 655, 685), textures2, Transform(), false));
	//Hammer
	vector<OGLTexture*> textures3;
	textures3.push_back((OGLTexture*)Assets::AssetManager::LoadTexture("hammer_gray.png"));
	textures3.push_back((OGLTexture*)Assets::AssetManager::LoadTexture("hammer.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((GameTechRenderer::GetRendererWidth() / 2) - 106, (GameTechRenderer::GetRendererWidth() / 2) - 42, 20, 84), textures3, Transform(), false));
	//Gun
	vector<OGLTexture*> textures4;
	textures4.push_back((OGLTexture*)Assets::AssetManager::LoadTexture("gun_gray.png"));
	textures4.push_back((OGLTexture*)Assets::AssetManager::LoadTexture("gun.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((GameTechRenderer::GetRendererWidth() / 2) - 32, (GameTechRenderer::GetRendererWidth() / 2) + 32, 20, 84), textures4, Transform(), false));
	//Bomb
	vector<OGLTexture*> textures5;
	textures5.push_back((OGLTexture*)Assets::AssetManager::LoadTexture("bomb_gray.png"));
	textures5.push_back((OGLTexture*)Assets::AssetManager::LoadTexture("bomb.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((GameTechRenderer::GetRendererWidth() / 2) + 42, (GameTechRenderer::GetRendererWidth() / 2) + 106, 20, 84), textures5, Transform(), false));
}

void GameTechRenderer::GenBuffers() {
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Generate Shadow FBO
	glGenFramebuffers(1, &shadowFBO);

	//Attach shadow texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Shader doesn't like this code for some reason?
	//Use 2 lines below to generate texture and FBO when shader draws correctly
	//shadowTex = OGLTexture::EmptyTexture(SHADOWSIZE, SHADOWSIZE, true);
	//GenerateFrameBuffer(&shadowFBO, nullptr, shadowTex);

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
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	ClearColor(Vector4(0.3f, 0.8f, 1, 1));
	BuildObjectList();
	SortObjectList();
	RenderShadowMap();
	RenderSkybox();
	RenderCamera();
	RenderLights();
	CombineBuffers();
	RenderHUD();
	glDisable(GL_CULL_FACE); //Todo - text indices are going the wrong way...
}

void GameTechRenderer::BuildObjectList() {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	gameWorld.GetObjectIterators(first, last);

	activeObjects.clear();

	for (std::vector<GameObject*>::const_iterator i = first; i != last; ++i) {
		if ((*i)->IsActive()) {
			const RenderObject*g = (*i)->GetComponent<RenderObject*>();
			if (g) {
				activeObjects.emplace_back(g);
			}
		}
	}
}

void GameTechRenderer::SortObjectList() {

}

void GameTechRenderer::RenderShadowMap() {
	BindFBO((void*)&shadowFBO);
	ClearBuffer(true, false, false);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	SetViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glCullFace(GL_FRONT);

	BindShader(shadowShader);

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(directionalLight->GetPosition(), Vector3(0, 0, 0));
	Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 5000.0f, 1, 45.0f);
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
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	BindFBO(nullptr);

	glCullFace(GL_BACK);
}

void GameTechRenderer::RenderSkybox() {
	BindFBO((void*)&gBufferFBO);
	ClearBuffer(true, true, false);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl *>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl *>()->BuildProjectionMatrix(screenAspect);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	BindShader(skyBoxShader);

	BindTextureCubeToShader((OGLTexture*)skybox, "cubeTex", 2);
	BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
	BindMatrix4ToShader(viewMatrix, "viewMatrix");
	BindMatrix4ToShader(projMatrix, "projMatrix");

	BindMesh(screenQuad);
	DrawBoundMesh();

	BindShader(nullptr);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
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

		for (int j = 0; j < currentMaterial->GetTextureParameters()->size(); j++)
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
	ClearColor(Vector4(0, 0, 0, 1));
	ClearBuffer(false, true, false);
	glBlendFunc(GL_ONE, GL_ONE);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetComponent<CameraControl*>()->BuildProjectionMatrix(screenAspect);

	BindShader(lightShader);

	Matrix4 identity;
	identity.ToIdentity();

	BindTextureToShader(gBufferDepthTex, "depthTex", 3);
	BindTextureToShader(gBufferNormalTex, "normTex", 4);

	for (int x = 0; x < 1; ++x) {
		float radius = directionalLight->GetRadius();

		Matrix4 tempModelMatrix = Matrix4::Translation(directionalLight->GetPosition())
			* directionalLight->GetOrientation().ToMatrix4()
			* Matrix4::Scale(Vector3(radius, radius, radius));

		//Need to be to bind vector2 to shader
		int pixelLocation = glGetUniformLocation(lightShader->GetProgramID(), "pixelSize");
		glUniform2f(pixelLocation, 1.0f / currentWidth, 1.0f / currentHeight);

		BindVector3ToShader(gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition(), "cameraPos");
		BindMatrix4ToShader(viewMatrix, "viewMatrix");
		BindMatrix4ToShader(projMatrix, "projMatrix");
		BindMatrix4ToShader(identity, "textureMatrix");
		BindVector3ToShader(directionalLight->GetPosition(), "lightPos");
		BindVector4ToShader(directionalLight->GetColour(), "lightColour");
		BindFloatToShader(radius, "lightRadius");
		BindFloatToShader(directionalLight->GetBrightness(), "lightBrightness");
		BindMatrix4ToShader(tempModelMatrix, "modelMatrix");
		BindMatrix4ToShader(shadowMatrix, "shadowMatrix");
		BindIntToShader(drawShadows, "drawShadows");

		//TODO: Overload BindTextureToShader to take GLuint as parameter, as above for all textures
		//BindTextureToShader(shadowTex, "shadowTex", 20);
		glUniform1i(glGetUniformLocation(lightShader->GetProgramID(),
			"shadowTex"), 20);
		glActiveTexture(GL_TEXTURE20);
		glBindTexture(GL_TEXTURE_2D, shadowTex);

		float dist = (directionalLight->GetPosition() - gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition()).Length();

		if (directionalLight->GetType() == LightType::Point) {
			if (dist < radius) {// camera is inside the light volume !
				glCullFace(GL_FRONT);
			}
			else {
				glCullFace(GL_BACK);
			}

			BindMesh(lightSphere);
		}
		else if (directionalLight->GetType() == LightType::Spot) {
			// Different calculation here to determine if inside the cone
			if (dist < radius) {// camera is inside the light volume !
				glCullFace(GL_FRONT);
			}
			else {
				glCullFace(GL_BACK);
			}

			//Change to bind cone.
			BindMesh(lightSphere);
		}

		//Calculates how many vertices are drawn per frame
		vertsDrawn += lightSphere->GetVertexCount();

		DrawBoundMesh();
	}

	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ClearColor(Vector4(0.2f, 0.2f, 0.2f, 1));

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
	for (int i = 0; i < hudObjects.size(); i++)
	{
		if (i == 0)
		{
			glEnable(GL_SCISSOR_TEST);
			glScissor(10 + 170 * (1- health), 180, 655, 685);
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
