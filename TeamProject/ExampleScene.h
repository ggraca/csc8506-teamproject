#pragma once

#include "GameScene.h"


class ExampleScene : public GameScene {
public:
	ExampleScene();
	void InitPlayer();

protected:
	void ResetWorld();
};