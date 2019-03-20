#pragma once

namespace NCL
{
	enum GamepadKeys
	{
		GAMEPAD_A = 0x04,
		GAMEPAD_B = 0x08,
		GAMEPAD_DPADDOWN = 0x128,
		GAMEPAD_DPADLEFT = 0x256,
		GAMEPAD_DPADRIGHT = 0x512,
		GAMEPAD_DPADUP = 0x64,
		GAMEPAD_LEFTSHOULDER = 0x1024,
		GAMEPAD_LEFTTHUMBSTICK = 0x4096,
		GAMEPAD_MENU = 0x01,
		GAMEPAD_NONE = 0x00,
		GAMEPAD_PADDLE1 = 0x16384,
		GAMEPAD_PADDLE2 = 0x32768,
		GAMEPAD_PADDLE3 = 0x65536,
		GAMEPAD_PADDLE4 = 0x131072,
		GAMEPAD_RIGHTSHOULDER = 0x2048,
		GAMEPAD_RIGHTTHUMBSTICK = 0x8192,
		GAMEPAD_VIEW = 0x02,
		GAMEPAD_X = 0x16,
		GAMEPAD_Y = 0x32,
		GAMEPAD_MAX = 0xFF
	};


	class Gamepad {
	public:
		friend class Window;

		//Is this key currently pressed down? 
		bool KeyDown(GamepadKeys key) const {
			return keyStates[key];
		}
		//Has this key been held down for multiple frames?
		bool KeyHeld(GamepadKeys key) const {
			if (KeyDown(key) && holdStates[key]) {
				return true;
			}
			return false;
		}
		//Is this the first update the key has been pressed for?
		bool KeyPressed(GamepadKeys key) const {
			return (KeyDown(key) && !KeyHeld(key));
		}

	protected:
		Gamepad();
		virtual ~Gamepad() {}

		void UpdateFrameState(float msec);

		void Sleep();
		void Wake();

		bool isAwake;
		bool keyStates[GAMEPAD_MAX];		//Is the key down?
		bool holdStates[GAMEPAD_MAX];		//Has the key been down for multiple updates?
	};

}
