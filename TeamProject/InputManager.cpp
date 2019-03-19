#include "InputManager.h"

//Is this the first update the key has been pressed for?
bool InputManager::IsButtonPressed(ActionButton actionButton)
{
	ButtonRelation relation = GetInstance().registeredActionButtons[actionButton];

	for (auto&i : relation.relatedKeyboardKeys)
	{
		if (Window::GetKeyboard()->KeyPressed(i)) { return true; }
	}
	for (auto&i : relation.relatedMouseKeys)
	{
		if (Window::GetMouse()->ButtonPressed(i)) { return true; }
	}

	return false;

}
//Has this key been held down for multiple frames?
bool InputManager::IsButtonHeld(ActionButton actionButton)
{
	ButtonRelation relation = GetInstance().registeredActionButtons[actionButton];

	for (auto&i : relation.relatedKeyboardKeys)
	{
		if (Window::GetKeyboard()->KeyHeld(i)) { return true; }
	}
	for (auto&i : relation.relatedMouseKeys)
	{
		if (Window::GetMouse()->ButtonHeld(i)) { return true; }
	}

	return false;
}

//Is this key currently pressed down?
bool InputManager::IsButtonDown(ActionButton actionButton)
{
	ButtonRelation relation = GetInstance().registeredActionButtons[actionButton];

	for (auto&i : relation.relatedKeyboardKeys)
	{
		if (Window::GetKeyboard()->KeyDown(i)) { return true; }
	}
	for (auto&i : relation.relatedMouseKeys)
	{
		if (Window::GetMouse()->ButtonDown(i)) { return true; }
	}
	
	return false;
}

//Is this mouse button double clicked since last frame
bool InputManager::IsDoubleClicked(ActionButton actionButton)
{
	ButtonRelation relation = GetInstance().registeredActionButtons[actionButton];

	for (auto&i : relation.relatedMouseKeys)
	{
		if (Window::GetMouse()->DoubleClicked(i)) { return true; }
	}

	return false;
}

void InputManager::Dispose() 
{
	delete[] GetInstance().registeredActionButtons;
}

short InputManager::GetInputBitsPressed()
{
	short result = 0;

	for (int i = ActionButton::FORWARD; i < ActionButton::MAX; i++)
	{
		short temp = IsButtonPressed(static_cast<ActionButton>(i)) << i;
		result |= temp;
	}

	return result;
}

short InputManager::GetInputBitsDown()
{
	short result = 0;

	for (int i = 0; i < ActionButton::MAX; i++)
	{
		short temp = IsButtonDown(static_cast<ActionButton>(i)) << i;
		result |= temp;
	}

	return result;
}

short InputManager::GetInputBitsHeld()
{
	short result = 0;

	for (int i = 0; i < ActionButton::MAX; i++)
	{
		short temp = IsButtonHeld(static_cast<ActionButton>(i)) << i;
		result |= temp;
	}

	return result;
}

short InputManager::GenerateShortFromContainer(InputContainer ic)
{
	short result = 0;

	for (int i = 0; i < ActionButton::MAX; i++)
	{
		short temp = ic.inputs[i] << i;
		result |= temp;
	}

	return result;
}


//Change content for action binding//
void InputManager::InitializeButtonRelations()
{
	GetInstance().registeredActionButtons = new ButtonRelation[ActionButton::MAX];
	GetInstance().registeredActionButtons[ActionButton::FORWARD].relatedKeyboardKeys.push_back(NCL::KEYBOARD_W);
	GetInstance().registeredActionButtons[ActionButton::FORWARD].relatedKeyboardKeys.push_back(NCL::KEYBOARD_UP);

	GetInstance().registeredActionButtons[ActionButton::BACKWARD].relatedKeyboardKeys.push_back(NCL::KEYBOARD_S);
	GetInstance().registeredActionButtons[ActionButton::BACKWARD].relatedKeyboardKeys.push_back(NCL::KEYBOARD_DOWN);

	GetInstance().registeredActionButtons[ActionButton::TOGGLE_GUN].relatedKeyboardKeys.push_back(NCL::KEYBOARD_1);
	GetInstance().registeredActionButtons[ActionButton::TOGGLE_HAMMER].relatedKeyboardKeys.push_back(NCL::KEYBOARD_2);
	GetInstance().registeredActionButtons[ActionButton::CALL_AIR_STRIKE].relatedKeyboardKeys.push_back(NCL::KEYBOARD_3);
	GetInstance().registeredActionButtons[ActionButton::TOGGLE_SHIELD].relatedMouseKeys.push_back(NCL::MOUSE_RIGHT);


	GetInstance().registeredActionButtons[ActionButton::LEFT].relatedKeyboardKeys.push_back(NCL::KEYBOARD_A);
	GetInstance().registeredActionButtons[ActionButton::LEFT].relatedKeyboardKeys.push_back(NCL::KEYBOARD_LEFT);

	GetInstance().registeredActionButtons[ActionButton::RIGHT].relatedKeyboardKeys.push_back(NCL::KEYBOARD_D);
	GetInstance().registeredActionButtons[ActionButton::RIGHT].relatedKeyboardKeys.push_back(NCL::KEYBOARD_RIGHT);

	GetInstance().registeredActionButtons[ActionButton::HIT].relatedMouseKeys.push_back(NCL::MOUSE_LEFT);

	GetInstance().registeredActionButtons[ActionButton::JUMP].relatedKeyboardKeys.push_back(NCL::KEYBOARD_SPACE);

	GetInstance().registeredActionButtons[ActionButton::DODGE].relatedKeyboardKeys.push_back(NCL::KEYBOARD_SHIFT);
}
