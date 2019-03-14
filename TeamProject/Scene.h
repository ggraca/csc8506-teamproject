#pragma once

#include "AudioEngine.h"
#include "BulletPhysics.h"
#include "GameTechRenderer.h"


class Scene {
public:
	Scene();
	virtual ~Scene();

	virtual void Update(float dt);

	GameWorld* GetGameWorld() const { return world; }
	// TODO: We need this for the HUD. But we should remove this once it is component based
	void SetRenderer(GameTechRenderer* gtr) { renderer = gtr; }
	

	BulletPhysics* physics; // TODO: Make this protected again?

protected:
	virtual void EarlyUpdate(float dt) {};
	virtual void LateUpdate(float dt) {};
	
	GameTechRenderer* renderer;
	GameWorld* world;
	CAudioEngine* audio;
	
};