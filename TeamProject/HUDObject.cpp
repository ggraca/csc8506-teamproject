#include "HUDObject.h"

HUDObject::HUDObject(OGLMesh* objMesh, vector<OGLTexture*> objTexture, Transform objTransform, OGLShader* objShader)
{
	objectMesh = objMesh;
	objectMesh->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	objectMesh->UploadToGPU();
	texture = objTexture;
	transform = objTransform;
	shader = objShader;
}

HUDObject::~HUDObject()
{
	delete objectMesh;
	for (int i = 0; i < texture.size(); i++)
	{
		delete texture[i];
	}
	delete shader;
}
