#pragma once

#include "GameScene.h"


class ExampleScene : public GameScene {
public:
	ExampleScene(bool& quitGame);

protected:
	void ResetWorld();
};
