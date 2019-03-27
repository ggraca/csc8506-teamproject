#pragma once

#include "GameScene.h"


class ExampleScene : public GameScene {
public:
	ExampleScene(Game* g, bool& quitGame);
	void InitPlayer();

protected:
	void ResetWorld();
};
