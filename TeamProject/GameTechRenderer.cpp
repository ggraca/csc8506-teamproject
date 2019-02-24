#include "GameTechRenderer.h"
#include "../TeamProject/GameObject.h"
#include "../Common/Camera.h"
#include "../Common/Vector2.h"
#include "../Common/Vector3.h"
#include "../Common/Matrix4.h"
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
	glDeleteFramebuffers(1, &gBufferFBO);
	glDeleteFramebuffers(1, &lightFBO);
	glDeleteFramebuffers(1, &shadowFBO);

	delete directionalLight;
}

void GameTechRenderer::AddHUDObjects()
{	

	//Green HealthBar
	vector<OGLTexture*> textures1;
	textures1.push_back((OGLTexture*)TextureLoader::LoadAPITexture("healthBarGreen.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad(180, 10, 655, 685), textures1, Transform(), new OGLShader("BasicVert.glsl", "BasicFrag.glsl")));
	//Red HealthBar
	vector<OGLTexture*> textures2;
	textures2.push_back((OGLTexture*)TextureLoader::LoadAPITexture("healthBarRed.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad(180, 10, 655, 685), textures2, Transform(), new OGLShader("BasicVert.glsl", "BasicFrag.glsl")));
	//Hammer
	vector<OGLTexture*> textures3;
	textures3.push_back((OGLTexture*)TextureLoader::LoadAPITexture("hammer_gray.png"));
	textures3.push_back((OGLTexture*)TextureLoader::LoadAPITexture("hammer.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((GameTechRenderer::GetRendererWidth() / 2) - 106, (GameTechRenderer::GetRendererWidth() / 2) - 42, 20, 84),
		textures3, Transform(), new OGLShader("BasicVert.glsl", "BasicFrag.glsl")));
	//Gun
	vector<OGLTexture*> textures4;
	textures4.push_back((OGLTexture*)TextureLoader::LoadAPITexture("gun_gray.png"));
	textures4.push_back((OGLTexture*)TextureLoader::LoadAPITexture("gun.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((GameTechRenderer::GetRendererWidth() / 2) - 32, (GameTechRenderer::GetRendererWidth() / 2) + 32, 20, 84),
		textures4, Transform(), new OGLShader("BasicVert.glsl", "BasicFrag.glsl")));
	//Bomb
	vector<OGLTexture*> textures5;
	textures5.push_back((OGLTexture*)TextureLoader::LoadAPITexture("bomb_gray.png"));
	textures5.push_back((OGLTexture*)TextureLoader::LoadAPITexture("bomb.png"));
	hudObjects.push_back(new HUDObject(OGLMesh::GenerateQuad((GameTechRenderer::GetRendererWidth() / 2) + 42, (GameTechRenderer::GetRendererWidth() / 2) + 106, 20, 84),
		textures5, Transform(), new OGLShader("BasicVert.glsl", "BasicFrag.glsl")));
	
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
	//Shader doesn't like this code for some reason?
	//shadowTex = OGLTexture::EmptyTexture(SHADOWSIZE, SHADOWSIZE, true);
	//Generate Shadow FBO
	glGenFramebuffers(1, &shadowFBO);

	//Attach shadow texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Generate G-Buffer FBO
	glGenFramebuffers(1, &gBufferFBO);

	GLenum GBuffer[3];
	GBuffer[0] = GL_COLOR_ATTACHMENT0;
	GBuffer[1] = GL_COLOR_ATTACHMENT1;
	GBuffer[2] = GL_COLOR_ATTACHMENT2;

	//Generate G-Buffer textures
	gBufferDepthTex = OGLTexture::EmptyTexture(currentWidth, currentHeight, true);
	gBufferColourTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	gBufferNormalTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	gBufferSpecularTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);

	// Add G-Buffer textures to G-Buffer FBO
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, ((OGLTexture*)gBufferColourTex)->GetObjectID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, ((OGLTexture*)gBufferNormalTex)->GetObjectID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
		GL_TEXTURE_2D, ((OGLTexture*)gBufferSpecularTex)->GetObjectID(), 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, ((OGLTexture*)gBufferDepthTex)->GetObjectID(), 0);
	glDrawBuffers(3, GBuffer);

	//Generate Light FBO
	glGenFramebuffers(1, &lightFBO);

	GLenum lightBuffer[2];
	lightBuffer[0] = GL_COLOR_ATTACHMENT0;
	lightBuffer[1] = GL_COLOR_ATTACHMENT1;

	//Generate light buffer textures
	lightEmissiveTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);
	lightSpecularTex = OGLTexture::EmptyTexture(currentWidth, currentHeight);

	//Add light textures to the light FBO
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, ((OGLTexture*)lightEmissiveTex)->GetObjectID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, ((OGLTexture*)lightSpecularTex)->GetObjectID(), 0);
	glDrawBuffers(2, lightBuffer);
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
			const RenderObject*g = (*i)->GetRenderObject();
			if (g) {
				activeObjects.emplace_back(g);
			}
		}
	}
}

void GameTechRenderer::SortObjectList() {

}

void GameTechRenderer::RenderShadowMap() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	ClearBuffer(true, false, false);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glCullFace(GL_FRONT);

	BindShader(shadowShader);
	int mvpLocation = glGetUniformLocation(shadowShader->GetProgramID(), "mvpMatrix");

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(directionalLight->GetPosition(), Vector3(0, 0, 0));
	Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 5000.0f, 1, 45.0f);

	Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;

	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on

	for (const auto&i : activeObjects) {
		Matrix4 modelMatrix = (*i).GetTransform()->GetWorldMatrix();
		Matrix4 mvpMatrix	= mvMatrix * modelMatrix;
		glUniformMatrix4fv(mvpLocation, 1, false, (float*)&mvpMatrix);
		BindMesh((*i).GetMesh());
		DrawBoundMesh();
	}

	// Calculates how many shadow casting lights are currently being renderered
	shadowCasters++;

	glViewport(0, 0, currentWidth, currentHeight);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void GameTechRenderer::RenderSkybox() {
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetScript<CameraControl *>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetScript<CameraControl *>()->BuildProjectionMatrix(screenAspect);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	ClearBuffer(true, true, false);

	BindShader(skyBoxShader);

	int cameraLocation = 0;
	int modelLocation = 0;
	int viewLocation = 0;
	int projLocation = 0;
	int textureLocation = 0;

	//TODO: Change to below
	//BindTextureToShader((OGLTexture*)skybox, "cubeTex", 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	int texLocation = glGetUniformLocation(skyBoxShader->GetProgramID(), "cubeTex");
	glUniform1i(texLocation, 2);

	cameraLocation = glGetUniformLocation(skyBoxShader->GetProgramID(), "cameraPos");
	modelLocation = glGetUniformLocation(skyBoxShader->GetProgramID(), "modelMatrix");
	viewLocation = glGetUniformLocation(skyBoxShader->GetProgramID(), "viewMatrix");
	projLocation = glGetUniformLocation(skyBoxShader->GetProgramID(), "projMatrix");
	textureLocation = glGetUniformLocation(skyBoxShader->GetProgramID(), "textureMatrix");

	Matrix4 identity;
	identity.ToIdentity();

	glUniform3fv(cameraLocation, 1, (float *)& gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition());
	glUniformMatrix4fv(modelLocation, 1, false, (float*)&identity);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(texLocation, 1, false, (float*)&identity);

	BindMesh(screenQuad);
	DrawBoundMesh();

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void GameTechRenderer::RenderCamera() {
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	ClearBuffer(true, false, false);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetScript<CameraControl*>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetScript<CameraControl*>()->BuildProjectionMatrix(screenAspect);

	OGLShader* activeShader = nullptr;
	int modelLocation	= 0;
	int viewLocation	= 0;
	int projLocation	= 0;
	int textureLocation = 0;
	int colourLocation  = 0;
	int cameraLocation = 0;

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

		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
		int texLocation = glGetUniformLocation(shader->GetProgramID(), "cubeTex");
		glUniform1i(texLocation, 8);

		if (activeShader != shader) {
			modelLocation	= glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
			viewLocation	= glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
			projLocation	= glGetUniformLocation(shader->GetProgramID(), "projMatrix");
			textureLocation = glGetUniformLocation(shader->GetProgramID(), "textureMatrix");
			colourLocation  = glGetUniformLocation(shader->GetProgramID(), "objectColour");

			cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");
			glUniform3fv(cameraLocation, 1, (float*)&gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition());//TODO give child position

			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

			activeShader = shader;
		}

		glUniformMatrix4fv(modelLocation, 1, false, (float*)&(*i).GetTransform()->GetWorldMatrix());
		glUniformMatrix4fv(textureLocation, 1, false, (float*)&currentMaterial->GetTextureMatrix());

		glUniform4fv(colourLocation, 1, (float*)&currentMaterial->GetColour());

		BindMesh((*i).GetMesh());

		//Calculates how many vertices are drawn per frame
		vertsDrawn += (*i).GetMesh()->GetVertexCount();

		DrawBoundMesh();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameTechRenderer::RenderLights() {
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	ClearColor(Vector4(0, 0, 0, 1));
	ClearBuffer(false, true, false);
	glBlendFunc(GL_ONE, GL_ONE);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetScript<CameraControl*>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetScript<CameraControl*>()->BuildProjectionMatrix(screenAspect);

	BindShader(lightShader);

	int cameraLocation = 0;
	int pixelLocation = 0;
	int lightPosLocation = 0;
	int lightColLocation = 0;
	int lightRadiusLocation = 0;
	int lightBrightnessLocation = 0;
	int modelLocation = 0;
	int viewLocation = 0;
	int projLocation = 0;
	int textureLocation = 0;
	int shadowLocation = 0;

	Matrix4 identity;
	identity.ToIdentity();

	//TODO: Overload BindTextureToShader to take GLuint as parameter, as above for all textures
	//BindTextureToShader(gBufferDepthTex, "depthTex", 3);
	//BindTextureToShader(gBufferNormalTex, "normTex", 4);
	glUniform1i(glGetUniformLocation(lightShader->GetProgramID(), "depthTex"), 3);
	glUniform1i(glGetUniformLocation(lightShader->GetProgramID(), "normTex"), 4);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)gBufferDepthTex)->GetObjectID());

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)gBufferNormalTex)->GetObjectID());

	cameraLocation = glGetUniformLocation(lightShader->GetProgramID(), "cameraPos");
	pixelLocation = glGetUniformLocation(lightShader->GetProgramID(), "pixelSize");
	viewLocation = glGetUniformLocation(lightShader->GetProgramID(), "viewMatrix");
	projLocation = glGetUniformLocation(lightShader->GetProgramID(), "projMatrix");
	textureLocation = glGetUniformLocation(lightShader->GetProgramID(), "textureMatrix");

	glUniform3fv(cameraLocation, 1, (float *)&gameWorld.GetMainCamera()->GetTransform().GetChildrenList()[0]->GetWorldPosition());
	glUniform2f(pixelLocation, 1.0f / currentWidth, 1.0f / currentHeight);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(textureLocation, 1, false, (float*)&identity);

	for (int x = 0; x < 1; ++x) {
		float radius = directionalLight->GetRadius();

		Matrix4 tempModelMatrix = Matrix4::Translation(directionalLight->GetPosition())
			* directionalLight->GetOrientation().ToMatrix4()
			* Matrix4::Scale(Vector3(radius, radius, radius));

		lightPosLocation = glGetUniformLocation(lightShader->GetProgramID(), "lightPos");
		lightColLocation = glGetUniformLocation(lightShader->GetProgramID(), "lightColour");
		lightRadiusLocation = glGetUniformLocation(lightShader->GetProgramID(), "lightRadius");
		lightBrightnessLocation = glGetUniformLocation(lightShader->GetProgramID(), "lightBrightness");
		modelLocation = glGetUniformLocation(lightShader->GetProgramID(), "modelMatrix");
		shadowLocation = glGetUniformLocation(lightShader->GetProgramID(), "shadowMatrix");

		glUniform3fv(lightPosLocation, 1, (float*)&directionalLight->GetPosition());
		glUniform4fv(lightColLocation, 1, (float*)&directionalLight->GetColour());
		glUniform1f(lightRadiusLocation, radius);
		glUniform1f(lightBrightnessLocation, directionalLight->GetBrightness());

		glUniformMatrix4fv(modelLocation, 1, false, (float*)&tempModelMatrix);
		glUniformMatrix4fv(shadowLocation, 1, false, (float*)&shadowMatrix);

		glUniform1i(glGetUniformLocation(lightShader->GetProgramID(),
			"drawShadows"), true);

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void GameTechRenderer::CombineBuffers() {
	////Keep this here for now ready for when post processes are to be implemented
	//glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	//	GL_TEXTURE_2D, postProcessTex[1], 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ClearBuffer(true, true, false);

	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetScript<CameraControl*>()->BuildViewMatrix();
	Matrix4 tempProjMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

	BindShader(combineShader);

	int modelLocation = 0;
	int viewLocation = 0;
	int projLocation = 0;
	int textureLocation = 0;
    int ambientLocation = 0;

	Matrix4 identity;
	identity.ToIdentity();

	modelLocation = glGetUniformLocation(combineShader->GetProgramID(), "modelMatrix");
	viewLocation = glGetUniformLocation(combineShader->GetProgramID(), "viewMatrix");
	projLocation = glGetUniformLocation(combineShader->GetProgramID(), "projMatrix");
	textureLocation = glGetUniformLocation(combineShader->GetProgramID(), "textureMatrix");
	ambientLocation = glGetUniformLocation(combineShader->GetProgramID(), "ambientColour");

	glUniformMatrix4fv(modelLocation, 1, false, (float*)&identity);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(projLocation, 1, false, (float*)&tempProjMatrix);
	glUniformMatrix4fv(textureLocation, 1, false, (float*)&identity);
	glUniform3f(ambientLocation, ambientColour.x, ambientColour.y, ambientColour.z);

	//Change to bind texture function
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "diffuseTex"), 2);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "depthTex"), 3);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "normTex"), 4);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "specularTex"), 5);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "emissiveTex"), 6);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "lightSpecularTex"), 7);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)gBufferColourTex)->GetObjectID());

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)gBufferDepthTex)->GetObjectID());

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)gBufferNormalTex)->GetObjectID());

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)gBufferSpecularTex)->GetObjectID());

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)lightEmissiveTex)->GetObjectID());

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, ((OGLTexture*)lightSpecularTex)->GetObjectID());

	BindMesh(screenQuad);
	DrawBoundMesh();
	
	glUseProgram(0);
}

void GameTechRenderer::RenderHUD()
{
	ClearBuffer(true, false, false);
	glDisable(GL_CULL_FACE);
	BindShader(hudShader);
	for (int i = 0; i < hudObjects.size(); i++)
	{
		glActiveTexture(GL_TEXTURE8);
		if (hudObjects[i]->GetTexture().size() == 1)
		{
			glBindTexture(GL_TEXTURE_2D, hudObjects[i]->GetTexture()[0]->GetObjectID());
		}
		else
		{
			if (true)
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
	}

	glEnable(GL_CULL_FACE);
}

void GameTechRenderer::SetupDebugMatrix(OGLShader*s) {
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->GetScript<CameraControl*>()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->GetScript<CameraControl*>()->BuildProjectionMatrix(screenAspect);

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