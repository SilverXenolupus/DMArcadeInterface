#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800

#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <XInput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsSpacePressed();
	float LeftRight();
	void GetMouseLocation(int&, int&);
	void GetJoyRotation(int&, int&);

	void Gamepad(int a_iIndex);
	int GamepadIndex();
	bool GamepadConnected();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	bool ReadJoystick();

	XINPUT_STATE ReadGamepad();

	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	IDirectInputDevice8* m_joystick;

	unsigned char m_keyboardState[256];
	unsigned char *m_PrevkeyboardState;
	DIMOUSESTATE m_mouseState;
	DIJOYSTATE2 m_joyState;
	XINPUT_STATE m_gamepadState;
	
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	short m_joyX, m_joyY;

	int m_iGamepadIndex;
};

#endif