#pragma once

#include "GameScene.h"


class ExampleScene : public GameScene {
public:
	ExampleScene();
	~ExampleScene() {};

protected:
	void ResetWorld();
};