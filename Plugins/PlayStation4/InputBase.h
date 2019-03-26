#pragma once
#include "pad.h"

namespace NCL {
	namespace PS4 {
		struct AXIS {
			float x;
			float y;

			AXIS() {
				x = 0.0f;
				y = 0.0f;
			}
		};

#define MAX_AXIS 8
#define MAX_BUTTONS 127

		class InputBase
		{
		public:
			InputBase();
			~InputBase();

			void ResetInput();

			AXIS	GetAxis(unsigned int i);
			float	GetButton(unsigned int i);
			bool	GetButtonDown(unsigned int i);

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

			void Sleep();
			void Wake();
			bool isAwake;
			void UpdateFrameState(float msec);
			bool keyStates[MAX_BUTTONS];		//Is the key down?
			bool holdStates[MAX_BUTTONS];		//Has the key been down for multiple updates?

		protected:

			AXIS	axis[MAX_AXIS];
			float   buttons[MAX_BUTTONS];
		};
	}
}
