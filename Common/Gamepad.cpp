#include "Gamepad.h"
#include <string>


using namespace NCL;


Gamepad::Gamepad()
{
	//Initialise the arrays to false!
	memset(keyStates, 0, MAX_BUTTONS * sizeof(bool));
	memset(holdStates, 0, MAX_BUTTONS * sizeof(bool));
}

/*
Updates variables controlling whether a keyboard key has been
held for multiple frames.
*/
//void Gamepad::UpdateFrameState(float msec) {
//	memcpy(holdStates, keyStates, MAX_BUTTONS * sizeof(bool));
//}

void Gamepad::Sleep() {
	isAwake = false;
	memset(keyStates, 0, MAX_BUTTONS * sizeof(bool));
	memset(holdStates, 0, MAX_BUTTONS * sizeof(bool));
}

void Gamepad::Wake() {
	isAwake = true;
}