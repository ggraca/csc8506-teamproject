#include "InputManager.h"



InputManager::InputManager()
{
	this->registeredActionButtons = new ButtonRelation[ActionButton::MAX];
	InitializeButtonRelations();
}


InputManager::~InputManager()
{
	delete[] registeredActionButtons;
}
//Is this the first update the key has been pressed for?
bool InputManager::IsButtonPressed(ActionButton actionButton)
{
	ButtonRelation relation = this->registeredActionButtons[actionButton];

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
	ButtonRelation relation = this->registeredActionButtons[actionButton];

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
	ButtonRelation relation = this->registeredActionButtons[actionButton];

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
	ButtonRelation relation = this->registeredActionButtons[actionButton];

	for (auto&i : relation.relatedMouseKeys)
	{
		if (Window::GetMouse()->DoubleClicked(i)) { return true; }
	}

	return false;
}


//Change content for action binding//
void InputManager::InitializeButtonRelations()
{
	registeredActionButtons[ActionButton::FORWARD].relatedKeyboardKeys.push_back(NCL::KEYBOARD_W);
	registeredActionButtons[ActionButton::FORWARD].relatedKeyboardKeys.push_back(NCL::KEYBOARD_UP);

	registeredActionButtons[ActionButton::BACKWARD].relatedKeyboardKeys.push_back(NCL::KEYBOARD_S);
	registeredActionButtons[ActionButton::BACKWARD].relatedKeyboardKeys.push_back(NCL::KEYBOARD_DOWN);

	registeredActionButtons[ActionButton::LEFT].relatedKeyboardKeys.push_back(NCL::KEYBOARD_A);
	registeredActionButtons[ActionButton::LEFT].relatedKeyboardKeys.push_back(NCL::KEYBOARD_LEFT);

	registeredActionButtons[ActionButton::RIGHT].relatedKeyboardKeys.push_back(NCL::KEYBOARD_D);
	registeredActionButtons[ActionButton::RIGHT].relatedKeyboardKeys.push_back(NCL::KEYBOARD_RIGHT);

	registeredActionButtons[ActionButton::FIRE].relatedMouseKeys.push_back(NCL::MOUSE_LEFT);
}
