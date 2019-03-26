#include "ExampleRenderer.h"
#include "RenderObject.h"
#include "../PlayStation4/PS4Shader.h"
#include "RenderObject.h"

using namespace NCL;
using namespace NCL::PS4;

ExampleRenderer::ExampleRenderer(PS4Window* window) : PS4RendererBase(window)
{
	rotation = 0.0f;
	defaultObject[0] = new RenderObjectPS4((MeshGeometry*)defaultMesh, (ShaderBase*)defaultShader, (TextureBase*)defaultTexture);
	defaultObject[1] = new RenderObjectPS4((MeshGeometry*)defaultMesh, (ShaderBase*)defaultShader, (TextureBase*)defaultTexture);
}

ExampleRenderer::~ExampleRenderer()
{
	delete defaultObject[0];
	delete defaultObject[1];
}

void ExampleRenderer::Update(float dt)	{
	rotation += dt;

	defaultObject[0]->SetLocalTransform(Mat4::Translation(Vec3(-0.4, 0, 0)) * Mat4::Rotation(rotation, Vec3(0,0,1)));

	defaultObject[1]->SetLocalTransform(Mat4::Translation(Vec3(0.4, 0, 0)));
}

void ExampleRenderer::RenderActiveScene() {
	DrawRenderObject(defaultObject[0]);
	DrawRenderObject(defaultObject[1]);
}

void ExampleRenderer::DrawRenderObject(RenderObjectPS4* o) {
	Mat4* transformMat = (Mat4*)currentGFXContext->allocateFromCommandBuffer(sizeof(Mat4), Gnm::kEmbeddedDataAlignment4);
	*transformMat = o->GetLocalTransform();

	Gnm::Buffer constantBuffer;
	constantBuffer.initAsConstantBuffer(transformMat, sizeof(Mat4));
	constantBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO); // it's a constant buffer, so read-only is OK

	PS4Shader* realShader = (PS4Shader*)o->GetShader();

	int objIndex = realShader->GetConstantBuffer("RenderObjectData");
	int camIndex = realShader->GetConstantBuffer("CameraData");

	currentGFXContext->setConstantBuffers(Gnm::kShaderStageVs, objIndex, 1, &constantBuffer);
	currentGFXContext->setConstantBuffers(Gnm::kShaderStageVs, camIndex, 1, &cameraBuffer);

	realShader->SubmitShaderSwitch(*currentGFXContext);

	DrawMesh(*defaultMesh);
}
