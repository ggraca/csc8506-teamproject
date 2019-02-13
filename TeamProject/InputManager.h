#pragma once
#include "ExampleScene"
#include <vector>

using namespace NCL;
using namespace CSC8503;


class InputManager
{
public:
	static enum ActionButton
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		FIRE,
		MAX
	};

	InputManager();
	~InputManager();

	
	bool IsButtonPressed(ActionButton actionButton);
	bool IsButtonHeld(ActionButton actionButton);
	bool IsButtonDown(ActionButton actionButton);
	bool IsDoubleClicked(ActionButton actionButton);

private:

	struct ButtonRelation
	{
		std::vector<NCL::KeyboardKeys> relatedKeyboardKeys;
		std::vector<NCL::MouseButtons> relatedMouseKeys;
	};

	void InitializeButtonRelations();

protected:

	ButtonRelation * registeredActionButtons;

};

