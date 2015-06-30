#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;
	m_Info->getInfo(names, picDir, gameDir, gameAmount);
	for (int i = 0; i < 8; i++)
	{
		int g = i;
		if (i < 4)
		{
			while (g >= gameAmount)
			{
				g -= gameAmount;
			}
			defaultG[i] = g;
		}
		if (i >= 4)
		{
			g = gameAmount - 8 + i;
			while (g < 0)
			{
				g += gameAmount;
			}
			while (g >= gameAmount)
			{
				g -= gameAmount;
			}
			defaultG[i] = g;
		}
		char gString[_MAX_DIR];
		_itoa(g, gString, 256);
		OutputDebugStringA(gString);

 	}

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.  If frame processing fails then exit.
			result = Frame();
			if (!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		// Check if the user pressed escape and wants to quit.
		if (m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}

	return;
}

bool SystemClass::Frame()
{
	bool result;

	// Do the input frame processing.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}
	//prevspace = space;
	space = m_Input->LeftRight();
	//if (prevspace == space)
	//{
	//	space = 0;
	//}
	// Get the location of the mouse from the input object,
	// Do the frame processing for the graphics object.


	changeR = false;
	changeL = false;

	if (space > 0.5f && pressOnce)
	{
		turn++;
		turnTimer = 15;
		pressOnce = false;
		leftSpin += 15;
		DMSpin += 50;
	}
	if (space < -0.5f && pressOnce)
	{
		turn--;
		turnTimer = 15;
		pressOnce = false;
		rightSpin += 15;
		DMSpin += 50;
	}
	if (space <= 0.1 && space >= -0.1)
	{
		pressOnce = true;
	}

	if (!pressOnce)
	{
		turnTimer--;
		if (turnTimer == 0)
		{
			turnTimer = 4;
			if (space > 0.5f)
			{
				turn++;
				leftSpin += 5;
				DMSpin += 30;
			}
			if (space < -0.5f)
			{
				turn--;
				rightSpin += 5;
				DMSpin += 30;
			}
		}
	}

	if (curTurn < turn)
	{
		changeL = true;
		curTurn = (int)turn;
	}
	if (curTurn > turn)
	{
		changeR = true;
		curTurn = (int)turn;
	}

	turnMin = (int)turn;

	while (turnMin > 7)
	{
		turnMin -= 8;
	}
	while (turnMin < 0)
	{
		turnMin += 8;
	}
	if (changeR)
		changeMin = 4 + turnMin;
	if (changeL)
		changeMin = 3 + turnMin;
	while (changeMin > 7)
	{
		changeMin -= 8;
	}
	while (changeMin < 0)
	{
		changeMin += 8;
	}
	if (changeR)
		changing = changeMin + 1;
	if (changeL)
		changing = changeMin - 1;
	while (changing > 7)
	{
		changing -= 8;
	}
	while (changing < 0)
	{
		changing += 8;
	}

	if (changeR)
	{
		int g = defaultG[changing] - 1;
		while (g < 0)
		{
			g += gameAmount;
		}
		defaultG[changeMin] = g;
	}
	if (changeL)
	{
		int g = defaultG[changing] + 1;
		while (g >= gameAmount)
		{
			g -= gameAmount;
		}
		defaultG[changeMin] = g;
	}

	//char gameNum[_MAX_DIR];
	//_itoa(defaultG[changeMin], gameNum, 256);
	//strcat(gameNum, "\n");
	//OutputDebugStringA(gameNum);

	result = m_Graphics->Frame(names, picDir, gameAmount, defaultG, turn, turnMin, leftSpin, rightSpin, DMSpin, toOpen);
	if (!result)
	{
		return false;
	}
	leftSpin -= D3DX_PI * min(5, (0.5f * leftSpin * (leftSpin / 100)));
	rightSpin -= D3DX_PI * min(5, (0.5f * rightSpin * (rightSpin / 100)));

	DMSpin -= D3DX_PI * min(5, (0.1f * DMSpin * (DMSpin / 100)));

	if (leftSpin < 0)
		leftSpin = 0;
	if (rightSpin < 0)
		rightSpin = 0;
	if (DMSpin < 1)
		DMSpin = 1;

	if (m_Input->IsSpacePressed() && !prevspace)
	{
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		/*************/
		//Get ShortCut
		/******/
		HRESULT hres;
		char Link[_MAX_DIR];
		wchar_t Temp[_MAX_DIR];
		strcpy(Link, gameDir[toOpen].c_str());
		mbstowcs(Temp, gameDir[toOpen].c_str(), _MAX_DIR);
		wchar_t Info[_MAX_DIR];

		IShellLink* psl;
		//Create the ShellLink object
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
			IID_IShellLink, (LPVOID*)&psl);

		if (SUCCEEDED(hres))
		{
			IPersistFile* ppf;
			//Bind the ShellLink object to the Persistent File
			hres = psl->QueryInterface(IID_IPersistFile, (LPVOID *)&ppf);
			if (SUCCEEDED(hres))
			{
				WCHAR wsz[MAX_PATH];
				//Get a UNICODE wide string wsz from the Link path
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Link, -1, wsz, MAX_PATH);

				//Read the link into the persistent file
				hres = ppf->Load(wsz, 0);

				if (SUCCEEDED(hres))
				{
					//Read the target information from the link object
					//UNC paths are supported (SLGP_UNCPRIORITY)
					psl->GetPath(Temp, 1024, NULL, SLGP_UNCPRIORITY);
					lstrcpyW(Info, Temp);
				}
			}
		}
		psl->Release();
		/********/

		wchar_t*  lpApplicationName = Info;


		if (!CreateProcess(NULL,   // No module name (use command line)
			lpApplicationName,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			return false;
		}

		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		//OutputDebugString(L"BOOP");
		
	}
	prevspace = m_Input->IsSpacePressed();
	// Finally render the graphics to the screen.
	/*result = m_Graphics->Render(0);
	if (!result)
	{
		return false;
	}*/

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"DMArcadeInterface";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 1920;
		screenHeight = 1080;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
					   PostQuitMessage(0);
					   return 0;
	}

		// Check if the window is being closed.
	case WM_CLOSE:
	{
					 PostQuitMessage(0);
					 return 0;
	}

		// All other messages pass to the message handler in the system class.
	default:
	{
			   return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
