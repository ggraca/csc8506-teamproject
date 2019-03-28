#pragma once
#pragma once
#include "GameTechRenderer.h"

//#include "../Common/TextureLoader.h"
//#include "../Plugins/OpenGLRendering/OGLTexture.h"
using namespace NCL;
#ifdef _WIN32
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
	OGLShader* shader;
	bool activeTexture;
};
#endif

#ifdef __ORBIS__
using namespace PS4;
class HUDObject
{
public:
	HUDObject(PS4Mesh* objMesh, vector<PS4Texture*> objTexture, Transform* objTransform, bool activeTex);

	~HUDObject();

	//void Update(const float dt, GameTechRenderer* renderer);
	PS4Mesh* GetObjectMesh() { return objectMesh; }
	vector<PS4Texture*> GetTexture() { return texture; }
	PS4Shader* GetShader() { return shader; }
	bool IsWeaponActive() { return activeTexture; }
	void SetWeaponState(bool state) { activeTexture = state; }


protected:
	PS4Mesh* objectMesh;
	vector<PS4Texture*> texture;
	Transform* transform;
	PS4Shader* shader = PS4Shader::GenerateShader("/app0/BasicVert.sb", "/app0/BasicFrag.sb");
	bool activeTexture;
};
#endif