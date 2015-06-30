#include "inputclass.h"

#pragma comment(lib, "Xinput.lib")


InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	m_joystick = 0;
}



InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;


	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	/*result = m_directInput->CreateDevice(GUID_Joystick, &m_joystick, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_joystick->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(result))
	{
		return false;
	}

	result = m_joystick->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_joystick->Acquire();
	if (FAILED(result))
	{
		return false;
	}*/

	return true;
}


void InputClass::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_joystick)
	{
		m_joystick->Unacquire();
		m_joystick->Release();
		m_joystick = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	m_gamepadState = ReadGamepad();

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;
	m_PrevkeyboardState = m_keyboardState;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadJoystick()
{
	HRESULT result;


	// Read the joystick device.
	result = m_joystick->GetDeviceState(sizeof(DIJOYSTATE2), (LPVOID)&m_joyState);
	if (FAILED(result))
	{
		// If the joystick lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_joystick->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

XINPUT_STATE InputClass::ReadGamepad()
{
	XINPUT_STATE tempState;
	ZeroMemory(&tempState, sizeof(XINPUT_STATE));
	XInputGetState(m_iGamepadIndex, &tempState);
	return tempState;
}

int InputClass::GamepadIndex()
{
	return m_iGamepadIndex;
}

bool InputClass::GamepadConnected()
{
	// Zero memory
	ZeroMemory(&m_gamepadState, sizeof(XINPUT_STATE));

	// Get the state of the gamepad
	DWORD Result = XInputGetState(m_iGamepadIndex, &m_gamepadState);

	if (Result == ERROR_SUCCESS)
		return true;  // The gamepad is connected
	else
		return false; // The gamepad is not connected
}

void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	//m_joyX = m_joyState.lRx;
	//m_joyY = m_joyState.lRy;

	//m_flightState
	m_joyX = m_gamepadState.Gamepad.sThumbLX;
	m_joyY = m_gamepadState.Gamepad.sThumbLY;
	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0)  { m_mouseX = 0; }
	if (m_mouseY < 0)  { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth)  { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_LCONTROL] & 0x80 && m_keyboardState[DIK_LALT] & 0x80 && m_keyboardState[DIK_Q] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsSpacePressed()
{	
	if (m_keyboardState[DIK_SPACE] & 0x80)
	{
		return true;
	}
	if (m_gamepadState.Gamepad.wButtons == XINPUT_GAMEPAD_A)
	{
		return true;
	}
	return false;
}

float InputClass::LeftRight()
{
	if (m_keyboardState[DIK_LEFT])
		return 1;
	else if (m_keyboardState[DIK_RIGHT])
		return -1;
	else
	{
		float dir;
		dir = m_gamepadState.Gamepad.sThumbLX;

		if ((float)m_gamepadState.Gamepad.sThumbLX > 32767)
			dir = 32767;
		if ((float)m_gamepadState.Gamepad.sThumbLX < -32767)
			dir = -32767;
		dir = dir / 32767;
		char padString[_MAX_DIR];
		//wsprintf(padString, L"%f", dir);
		_itoa((int)dir, padString, 10);
		OutputDebugStringA(padString);

		return -dir;
	}
	//m_gamepadState.Gamepad.sThumbLX;

}


void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

void InputClass::GetJoyRotation(int& joyX, int& joyY)
{
	joyX = m_joyX;
	joyY = m_joyY;
	return;
}