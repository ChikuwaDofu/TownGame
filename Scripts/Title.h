#pragma once

#include"Suken.h"
#include"Picture.h"

const int HTP = 9;

class CTitle{
private:
	static CPicture g_title;
	static CPicture g_newGame;
	static CPicture g_newGameOn;
	static CPicture g_loadGame;
	static CPicture g_loadGameOn;
	static CPicture g_HtP[HTP];
	static CPicture g_htpB;
	static CPicture g_htpBOn;
	static CPicture g_AR;
	static CPicture g_AL;

	bool hFlag;
	int htNum;

public:
	bool newGame;
	bool loadGame;

	CTitle();
	void Set();
	void Draw();
};
