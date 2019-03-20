#include "Gamepad.h"
#include <string>


using namespace NCL;


Gamepad::Gamepad()
{
	//Initialise the arrays to false!
	memset(keyStates, 0, GAMEPAD_MAX * sizeof(bool));
	memset(holdStates, 0, GAMEPAD_MAX * sizeof(bool));
}

/*
Updates variables controlling whether a keyboard key has been
held for multiple frames.
*/
void Gamepad::UpdateFrameState(float msec) {
	memcpy(holdStates, keyStates, GAMEPAD_MAX * sizeof(bool));
}

void Gamepad::Sleep() {
	isAwake = false;
	memset(keyStates, 0, GAMEPAD_MAX * sizeof(bool));
	memset(holdStates, 0, GAMEPAD_MAX * sizeof(bool));
}

void Gamepad::Wake() {
	isAwake = true;
}