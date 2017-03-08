#pragma once

#include"Suken.h"
#include"Picture.h"

class CFoundBox{
private:
	static CPicture g_cross;
	static CPicture g_town;
	bool close;
	bool select;
	char tType;

public:
	CFoundBox();
	void LoadGraph();
	void DrawFB();
	void DrawButton(char type, int x, int y);
	bool GetClose();
	bool GetSelect();
	char GetTType();
};