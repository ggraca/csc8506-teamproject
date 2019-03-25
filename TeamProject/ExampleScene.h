#pragma once

#include "GameScene.h"


class ExampleScene : public GameScene {
public:
	ExampleScene(bool& quitGame);
	ExampleScene();
	void InitPlayer();

protected:
	void ResetWorld();
};
