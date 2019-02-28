#pragma once
#pragma once
#include "GameTechRenderer.h"
//#include "../Common/TextureLoader.h"
//#include "../Plugins/OpenGLRendering/OGLTexture.h"
using namespace NCL;
class HUDObject
{
public:
	HUDObject(OGLMesh* objMesh, vector<OGLTexture*> objTexture, Transform objTransform, bool activeTex);

	~HUDObject();

	//void Update(const float dt, GameTechRenderer* renderer);
	OGLMesh* GetObjectMesh() { return objectMesh; }
	vector<OGLTexture*> GetTexture() { return texture; }
	OGLShader* GetShader() { return shader; }
	bool IsWeaponActive() { return activeTexture; }
	void SetWeaponState(bool state) { activeTexture = state; }

	
protected:
	OGLMesh* objectMesh;
	vector<OGLTexture*> texture;
	Transform transform;
	OGLShader* shader = new OGLShader("BasicVert.glsl", "BasicFrag.glsl");
	bool activeTexture;
};