#pragma once
#include "pad.h"
#include "../Plugins/PlayStation4/InputBase.h"

namespace NCL
{
	class Gamepad {
	public:
		friend class Window;

		//Is this key currently pressed down? 
		bool KeyDown(ScePadButtonDataOffset key) const {
			return keyStates[key];
		}
		//Has this key been held down for multiple frames?
		bool KeyHeld(ScePadButtonDataOffset key) const {
			if (KeyDown(key) && holdStates[key]) {
				return true;
			}
			return false;
		}
		//Is this the first update the key has been pressed for?
		bool KeyPressed(ScePadButtonDataOffset key) const {
			return (KeyDown(key) && !KeyHeld(key));
		}

	protected:
		Gamepad();
		virtual ~Gamepad() {}

		//void UpdateFrameState(float msec);

		void Sleep();
		void Wake();

		bool isAwake;
		bool keyStates[MAX_BUTTONS];		//Is the key down?
		bool holdStates[MAX_BUTTONS];		//Has the key been down for multiple updates?
	};

}
