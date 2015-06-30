#include "GameInfo.h"

GameInfo::GameInfo()
{

}

GameInfo::GameInfo(const GameInfo& other)
{
}


GameInfo::~GameInfo()
{
}

void GameInfo::getInfo(string names[], string picDir[], string gameDir[], int& gameAmount)
{
	wchar_t docs[MAX_PATH];
	char * gameFolder;
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	string gameFolders[260];
	DIR *pdir = NULL;

	int i;

	SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, docs);
	
	gameFolder = (char*)malloc(64);

	wcstombs(gameFolder, docs, 64);

	strcat(gameFolder, "\\__GAMES\\");

	mbstowcs(szDir, gameFolder, 64);

	i = 0;

	pdir = opendir(gameFolder);
	struct dirent *pent = NULL;
	while (pent = readdir(pdir))
	{
		gameFolders[i] = pent->d_name;
		i++;
	}

	closedir(pdir);

	gameAmount = i - 2;

	for (int j = 0; j < i-2; j++)
	{
		char pictureDir[_MAX_DIR];
		char gamesDir[_MAX_DIR];

		wcstombs(pictureDir, docs, 64);
		strcat(pictureDir, "\\__GAMES\\");
		strcat(pictureDir, gameFolders[j+2].c_str());
		strcat(pictureDir, "\\PIC.dds");

		picDir[j] = pictureDir;

		wcstombs(gamesDir, docs, 64);
		strcat(gamesDir, "\\__GAMES\\");
		strcat(gamesDir, gameFolders[j+2].c_str());
		strcat(gamesDir, "\\GAME.lnk");

		gameDir[j] = gamesDir;

		char name[_MAX_DIR];
		strcpy(name, gameFolders[j+2].c_str());

		names[j] = name;
	}

	return;

}