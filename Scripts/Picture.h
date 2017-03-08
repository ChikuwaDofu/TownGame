#pragma once

#include"Suken.h"

class CPicture{
private:
	std::vector<int> handle;

public:
	CPicture(){}
	CPicture(const char* file);
	CPicture(const char* file, int xNum, int yNum, int xSize, int ySize, int num);
	void Draw(int x, int y);
	void Draw(int x, int y, int num);
	void Load(const char* file);
	void Load(const char* file, int xNum, int yNum, int xSize, int ySize, int num);
};
