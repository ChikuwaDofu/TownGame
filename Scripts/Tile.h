#pragma once

#include"Suken.h"
#include"Picture.h"

const int GRID = 60;
const char PLAIN = 0;
const char SEA = 1;
const char NO_TOWN = -1;
const char FARM = 0;

class CTile{
private:
	static CPicture g_tile;
	static CPicture g_town;
	static CPicture g_frame;
	static CPicture g_cross;
	int tX;
	int tY;
	char terrain;
	char town;
	bool info;
	bool clickInfo;

public:
	CTile();
	void Set(int x, int y, char num);
	void Draw();
	static void LoadGraph();
	void DrawInfo();
	bool GetInfo();
	char GetTown();
	void SetTown(char tNum);
};
