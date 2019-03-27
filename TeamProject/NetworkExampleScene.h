#pragma once

#include "GameScene.h"


class NetworkExampleScene : public GameScene {
public:
	NetworkExampleScene(Game* g, bool& quitGame);

protected:
	void ResetWorld();
};