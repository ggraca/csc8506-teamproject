#include "HUDObject.h"
#ifdef _WIN32
#include "../Common/Assets.h"

HUDObject::HUDObject(OGLMesh* objMesh, vector<OGLTexture*> objTexture, Transform objTransform, bool activeTex)
{
	objectMesh = objMesh;
	objectMesh->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	objectMesh->UploadToGPU();
	texture = objTexture;
	transform = objTransform;
	activeTexture = activeTex;
	shader = (OGLShader*)Assets::AssetManager::LoadShader("BasicShader", "BasicVert.glsl", "BasicFrag.glsl");
}
#endif

#ifdef __ORBIS__
HUDObject::HUDObject(PS4Mesh* objMesh, vector<PS4Texture*> objTexture, Transform* objTransform, bool activeTex)
{
	objectMesh = objMesh;
	objectMesh->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	objectMesh->UploadToGPU();
	texture = objTexture;
	transform = objTransform;
	activeTexture = activeTex;
	//shader = objShader;
}
#endif


HUDObject::~HUDObject()
{
	delete objectMesh;
	for (unsigned int i = 0; i < texture.size(); i++)
	{
		delete texture[i];
	}
	delete shader;
}
