#ifndef _GAMEINFO_H_
#define _GAMEINFO_H_
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <dirent.h>
#include <shlobj.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#pragma comment(lib, "User32.lib")
using namespace std;

class GameInfo
{
public:
	GameInfo();
	GameInfo(const GameInfo&);
	~GameInfo();

	void getInfo(string[], string[], string[], int&);

private:

};
#endif