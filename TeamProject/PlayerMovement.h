#pragma once

#include "ScriptObject.h"

class PlayerMovement : virtual public ScriptObject {
public:
	void Update(float dt) override;

private:
	float movementSpeed = 200;
	float jumpSpeed = 400;
};

