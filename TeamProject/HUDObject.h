#pragma once
#pragma once
#include "GameTechRenderer.h"
//#include "../Common/TextureLoader.h"
//#include "../Plugins/OpenGLRendering/OGLTexture.h"
using namespace NCL;
class HUDObject
{
public:
	HUDObject(OGLMesh* objMesh, vector<OGLTexture*> objTexture, Transform objTransform, OGLShader* objShader);

	~HUDObject();

	//void Update(const float dt, GameTechRenderer* renderer);
	OGLMesh* GetObjectMesh() { return objectMesh; }
	vector<OGLTexture*> GetTexture() { return texture; }
	OGLShader* GetShader() { return shader; }

	
protected:
	OGLMesh* objectMesh;
	vector<OGLTexture*> texture;
	Transform transform;
	OGLShader* shader;
};