#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <strsafe.h>

#include "inputclass.h"
#include "graphicsclass.h"
#include "GameInfo.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	GameInfo* m_Info;

	int defaultG[8];

	float space;
	float prevspace;
	int toOpen;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	bool changeR = false;
	bool changeL = false;
	bool pressOnce = true;
	int turnTimer = 1000;
	int turn = 0;
	int curTurn = 0;

	int turnMin = 0;
	int changeMin = 0;
	int changing = 0;

	float leftSpin = 0;
	float rightSpin = 0;
	float DMSpin;

	string names[256];
	string picDir[256];
	string gameDir[256];
	int gameAmount;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif