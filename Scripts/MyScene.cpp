#include"MyScene.h"
#include"TileM.h"
#include"Tile.h"

CTileManager tileM;

void SetClass(){
	tileM.Set();
}

void LoadHandle(){
	CTile::LoadGraph();
}

void DrawGame(){
	tileM.Draw();
}