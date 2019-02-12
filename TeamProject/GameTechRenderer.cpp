#include "GameTechRenderer.h"
#include "../GameTechCommon/GameObject.h"
#include "../Common/Camera.h"
#include "../Common/Vector2.h"
#include "../Common/Vector3.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

#define SHADOWSIZE 4096

Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5, 0.5, 0.5)) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));

GameTechRenderer::GameTechRenderer(GameWorld& world) : OGLRenderer(*Window::GetWindow()), gameWorld(world)	{
	glEnable(GL_DEPTH_TEST);

	shadowShader = new OGLShader("GameTechShadowVert.glsl", "GameTechShadowFrag.glsl");
	skyBoxShader = new OGLShader("skyboxVertex.glsl", "skyboxFragment.glsl");
	lightShader = new OGLShader("pointlightvert.glsl", "pointlightfrag.glsl");
	combineShader = new OGLShader("combinevert.glsl", "combinefrag.glsl");

	GenBuffers();

	screenQuad = OGLMesh::GenerateQuad();
	screenQuad->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	screenQuad->UploadToGPU();

	glClearColor(1, 1, 1, 1);

	//Set up the light properties
	lightColour = Vector4(1.0f, 1.0f, 0.5f, 1.0f);
	lightRadius = 4000.0f;
	lightPosition = Vector3(-2000.0f, 2500.0f, -2000.0f);
}

GameTechRenderer::~GameTechRenderer()	{
	glDeleteTextures(1, &gBufferDepthTex);
	glDeleteTextures(1, &gBufferColourTex);
	glDeleteTextures(1, &gBufferNormalTex);
	glDeleteTextures(1, &gBufferSpecularTex);
	glDeleteTextures(1, &lightEmissiveTex);
	glDeleteTextures(1, &lightSpecularTex);
	glDeleteTextures(1, &shadowTex);

	glDeleteFramebuffers(1, &gBufferFBO);
	glDeleteFramebuffers(1, &lightFBO);
	glDeleteFramebuffers(1, &shadowFBO);
}

void GameTechRenderer::GenBuffers() {
	#pragma region ShadowFBORegion

	//Generate shadow texture and assign texture parameters
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

	#pragma endregion

	#pragma region GBufferFBORegion

	//Generate G-Buffer FBO
	glGenFramebuffers(1, &gBufferFBO);

	GLenum GBuffer[3]; // was 5, testing if it breaks as shaders only draw to 3 colour attachments
	GBuffer[0] = GL_COLOR_ATTACHMENT0;
	GBuffer[1] = GL_COLOR_ATTACHMENT1;
	GBuffer[2] = GL_COLOR_ATTACHMENT2;
	//GBuffer[3] = GL_COLOR_ATTACHMENT3;
	//GBuffer[4] = GL_COLOR_ATTACHMENT4;

	//Generate G-Buffer textures
	GenerateScreenTexture(gBufferDepthTex, true);
	GenerateScreenTexture(gBufferColourTex);
	GenerateScreenTexture(gBufferNormalTex);
	GenerateScreenTexture(gBufferSpecularTex);

	// Add G-Buffer textures to G-Buffer FBO
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, gBufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, gBufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
		GL_TEXTURE_2D, gBufferSpecularTex, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, gBufferDepthTex, 0);
	glDrawBuffers(3, GBuffer);

	#pragma endregion

	#pragma region LightFBORegion
	
	//Generate Light FBO
	glGenFramebuffers(1, &lightFBO);

	GLenum lightBuffer[2];
	lightBuffer[0] = GL_COLOR_ATTACHMENT0;
	lightBuffer[1] = GL_COLOR_ATTACHMENT1;

	//Generate light buffer textures
	GenerateScreenTexture(lightEmissiveTex);
	GenerateScreenTexture(lightSpecularTex);

	//Add light textures to the light FBO
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, lightEmissiveTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, lightBuffer);

	#pragma endregion

}

void GameTechRenderer::GenerateScreenTexture(GLuint & into, bool depth) {
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8, currentWidth, currentHeight, 0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameTechRenderer::RenderFrame() {
	glEnable(GL_CULL_FACE);
	glClearColor(0.3f, 0.8f, 1, 1);
	BuildObjectList();
	SortObjectList();
	RenderShadowMap();
	//RenderSkybox(); Don't call for now
	RenderCamera();
	RenderLights();
	CombineBuffers();
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
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glCullFace(GL_FRONT);

	BindShader(shadowShader);
	int mvpLocation = glGetUniformLocation(shadowShader->GetProgramID(), "mvpMatrix");

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(lightPosition, Vector3(0, 0, 0));
	Matrix4 shadowProjMatrix = Matrix4::Perspective(1800.0f, 5000.0f, 1, 45.0f);

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
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	BindShader(skyBoxShader);

	glUniform3fv(glGetUniformLocation(skyBoxShader->GetProgramID(),
		"cameraPos"), 1, (float *)& gameWorld.GetMainCamera()->GetPosition());
	glUniform1i(glGetUniformLocation(skyBoxShader->GetProgramID(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);

	Matrix4 identity;
	identity.ToIdentity();

	glUniformMatrix4fv(glGetUniformLocation(skyBoxShader->GetProgramID(),
		"modelMatrix"), 1, false, (float*)&identity);
	glUniformMatrix4fv(glGetUniformLocation(skyBoxShader->GetProgramID(),
		"viewMatrix"), 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(skyBoxShader->GetProgramID(),
		"projMatrix"), 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(glGetUniformLocation(skyBoxShader->GetProgramID(),
		"textureMatrix"), 1, false, (float*)&identity);
	
	//Need a way to import a quad and render the skybox to this quad
	//The quad should be the size of the screen
	//screen->Draw();

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
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	OGLShader* activeShader = nullptr;
	int projLocation	= 0;
	int viewLocation	= 0;
	int modelLocation	= 0;
	int textureLocation = 0;
	int colourLocation  = 0;

	int cameraLocation = 0;
	shadowCasters = 0;
	vertsDrawn = 0;

	//TODO - PUT IN FUNCTION
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	for (const auto&i : activeObjects) {
		OGLShader* shader = (OGLShader*)(*i).GetShader();
		BindShader(shader);

		BindTextureToShader((OGLTexture*)(*i).GetDefaultTexture(), "mainTex", 0);

		if (activeShader != shader) {
			projLocation	= glGetUniformLocation(shader->GetProgramID(), "projMatrix");
			viewLocation	= glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
			modelLocation	= glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
			textureLocation = glGetUniformLocation(shader->GetProgramID(), "textureMatrix");
			//shadowLocation  = glGetUniformLocation(shader->GetProgramID(), "shadowMatrix");
			colourLocation  = glGetUniformLocation(shader->GetProgramID(), "objectColour");

			cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");
			glUniform3fv(cameraLocation, 1, (float*)&gameWorld.GetMainCamera()->GetPosition());

			glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

			//int shadowTexLocation = glGetUniformLocation(shader->GetProgramID(), "shadowTex");
			//glUniform1i(shadowTexLocation, 1);

			activeShader = shader;
		}

		Matrix4 modelMatrix = (*i).GetTransform()->GetWorldMatrix();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		Matrix4 textureMatrix;
		textureMatrix.ToIdentity();
		glUniformMatrix4fv(textureLocation, 1, false, (float*)&textureMatrix);

		//Matrix4 fullShadowMat = shadowMatrix * modelMatrix;
		//glUniformMatrix4fv(shadowLocation, 1, false, (float*)&fullShadowMat);

		glUniform4fv(colourLocation, 1, (float*)&i->GetColour());

		BindMesh((*i).GetMesh());

		//Calculates how many vertices are drawn per frame
		vertsDrawn += (*i).GetMesh()->GetVertexCount();

		DrawBoundMesh();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameTechRenderer::RenderLights() {
	glUseProgram(lightShader->GetProgramID());
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	glUniform1i(glGetUniformLocation(lightShader->GetProgramID(), "depthTex"), 3);
	glUniform1i(glGetUniformLocation(lightShader->GetProgramID(), "normTex"), 4);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gBufferDepthTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gBufferNormalTex);

	glUniform3fv(glGetUniformLocation(lightShader->GetProgramID(), "cameraPos"), 1, (float *)&gameWorld.GetMainCamera()->GetPosition());

	glUniform2f(glGetUniformLocation(lightShader->GetProgramID(), "pixelSize"),
		1.0f / currentWidth, 1.0f / currentHeight);

	for (int x = 0; x < 1; ++x) {
		float radius = lightRadius;

		Matrix4 tempModelMatrix = Matrix4::Translation(lightPosition) * Matrix4::Scale(Vector3(radius, radius, radius));
		lightPosition = tempModelMatrix.GetPositionVector();

		glUniform3fv(glGetUniformLocation(lightShader->GetProgramID(),
			"lightPos"), 1, (float*)&lightPosition);
		glUniform4fv(glGetUniformLocation(lightShader->GetProgramID(),
			"lightColour"), 1, (float*)&lightColour);
		glUniform1f(glGetUniformLocation(lightShader->GetProgramID(),
			"lightRadius"), radius);
		glUniform1f(glGetUniformLocation(lightShader->GetProgramID(),
			"lightBrightness"), lightBrightness);

		Matrix4 identity;
		identity.ToIdentity();

		glUniformMatrix4fv(glGetUniformLocation(lightShader->GetProgramID(),
			"modelMatrix"), 1, false, (float*)&tempModelMatrix);
		glUniformMatrix4fv(glGetUniformLocation(lightShader->GetProgramID(),
			"viewMatrix"), 1, false, (float*)&viewMatrix);
		glUniformMatrix4fv(glGetUniformLocation(lightShader->GetProgramID(),
			"projMatrix"), 1, false, (float*)&projMatrix);
		glUniformMatrix4fv(glGetUniformLocation(lightShader->GetProgramID(),
			"textureMatrix"), 1, false, (float*)&identity);
		glUniformMatrix4fv(glGetUniformLocation(lightShader->GetProgramID(),
			"shadowMatrix"), 1, false, (float*)&shadowMatrix);

		glUniform1i(glGetUniformLocation(lightShader->GetProgramID(),
			"shadowTex"), 20);
		glUniform1i(glGetUniformLocation(lightShader->GetProgramID(),
			"drawShadows"), true);
		glActiveTexture(GL_TEXTURE20);
		glBindTexture(GL_TEXTURE_2D, shadowTex);

		float dist = (lightPosition - gameWorld.GetMainCamera()->GetPosition()).Length();
		if (dist < radius) {// camera is inside the light volume !
			glCullFace(GL_FRONT);
		}
		else {
			glCullFace(GL_BACK);
		}

		BindMesh(lightSphere);

		//Calculates how many vertices are drawn per frame
		vertsDrawn += lightSphere->GetVertexCount();

		DrawBoundMesh();
	}

	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void GameTechRenderer::CombineBuffers() {
	glUseProgram(combineShader->GetProgramID());

	////Keep this here for now ready for when post processes are to be implemented
	//glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	//	GL_TEXTURE_2D, postProcessTex[1], 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);

	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 tempProjMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

	Matrix4 identity;
	identity.ToIdentity();

	glUniformMatrix4fv(glGetUniformLocation(combineShader->GetProgramID(),
		"modelMatrix"), 1, false, (float*)&identity);
	glUniformMatrix4fv(glGetUniformLocation(combineShader->GetProgramID(),
		"viewMatrix"), 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(combineShader->GetProgramID(),
		"projMatrix"), 1, false, (float*)&tempProjMatrix);
	glUniformMatrix4fv(glGetUniformLocation(combineShader->GetProgramID(),
		"textureMatrix"), 1, false, (float*)&identity);

	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "diffuseTex"), 2);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "depthTex"), 3);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "normTex"), 4);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "specularTex"), 5);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "emissiveTex"), 6);
	glUniform1i(glGetUniformLocation(combineShader->GetProgramID(), "lightSpecularTex"), 7);


	glUniform3f(glGetUniformLocation(combineShader->GetProgramID(),
		"ambientColour"), ambientColour.x, ambientColour.y, ambientColour.z);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBufferColourTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gBufferDepthTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gBufferNormalTex);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, gBufferSpecularTex);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, lightEmissiveTex);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

	BindMesh(screenQuad);
	DrawBoundMesh();

	glUseProgram(0);
}

void GameTechRenderer::SetupDebugMatrix(OGLShader*s) {
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	Matrix4 vp = projMatrix * viewMatrix;

	int matLocation = glGetUniformLocation(s->GetProgramID(), "viewProjMatrix");

	glUniformMatrix4fv(matLocation, 1, false, (float*)&vp);
}
