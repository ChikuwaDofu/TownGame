#include"Tile.h"

CPicture CTile::g_tile;
CPicture CTile::g_town;
CPicture CTile::g_frame;
CPicture CTile::g_cross;

CTile::CTile(){
	info=false;
	clickInfo=false;
	town=0;
}

void CTile::Set(int x, int y, char num){
	tX=x;
	tY=y;
	terrain=num;
	town=NO_TOWN;
}

void CTile::LoadGraph(){
	g_tile.Load("Chikuwa3/Tiles.png", 2, 2, GRID, GRID, 4);
	g_town.Load("Chikuwa3/Towns.png", 2, 2, GRID, GRID, 4);
	g_frame.Load("Chikuwa3/Frame.png");
	g_cross.Load("Chikuwa3/X.png");
}

void CTile::Draw(){
	info = false;

	g_tile.Draw(tX, tY, terrain);

	if(town != NO_TOWN){
		g_town.Draw(tX, tY, town);
	}

	if(Event.RMouse.GetOn(tX-1, tY-1, tX+GRID, tY+GRID)){
		g_frame.Draw(tX, tY);
	}
	
	if(Event.LMouse.GetClick(tX-1, tY-1, tX+GRID, tY+GRID)){
		info=true;
		clickInfo=true;
	}
}

//void CTile::DrawInfo(){
//	if(info){
//		DrawBox(300, 0, 900, 600, WHITE, true);
//
//		g_cross.Draw(860, 0);
//
//		if(Event.LMouse.GetClick(859, -1, 900, 40) && !clickInfo){
//			info=false;
//		}
//	}
//
//	clickInfo=false;
//}

bool CTile::GetInfo(){
	return info;
}

char CTile::GetTown(){
	return town;
}

void CTile::SetTown(char tNum){
	town = tNum;
}