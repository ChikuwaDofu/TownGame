#pragma once

#include"Suken.h"
#include"Tile.h"
#include"InfoBox.h"

const int BLOCKS_X = 10;
const int BLOCKS_Y = 10;

class CTileManager{
private:
	CTile tile[100];//0 1
					//2 3
	CFoundBox *fbox;
	bool info;
	int infoNum;

public:
	CTileManager(){}
	void Set();
	void Draw();
	void CheckInfo();
};