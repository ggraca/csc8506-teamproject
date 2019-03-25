#pragma once

#include "GameScene.h"


class NetworkExampleScene : public GameScene {
public:
	NetworkExampleScene(bool& quitGame);

protected:
	void ResetWorld();
};