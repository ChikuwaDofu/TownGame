#include"InfoBox.h"

CPicture CFoundBox::g_cross;
CPicture CFoundBox::g_town;

CFoundBox::CFoundBox(){
	g_cross.Load("Chikuwa3/X.png");
	g_town.Load("Chikuwa3/Towns.png", 2, 2, 60, 60, 4);

	close = false;
	select = false;
}

void CFoundBox::LoadGraph(){
	g_cross.Load("Chikuwa3/X.png");
	g_town.Load("Chikuwa3/Towns.png", 2, 2, 60, 60, 4);
}

void CFoundBox::DrawFB(){
	DrawBox(300, 0, 900, 600, WHITE, true);

	g_cross.Draw(860, 0);

	if(Event.LMouse.GetClick(859, -1, 900, 40)){
		close = true;
	}

	DrawButton(0, 570, 270);
}

void CFoundBox::DrawButton(char type, int x, int y){
	g_town.Draw(x, y, type);

	if(Event.LMouse.GetClick(x - 1, y - 1, x + 60, y+ 60)){
		tType = type;
		select = true;
	}
}

bool CFoundBox::GetClose(){
	return close;
}

bool CFoundBox::GetSelect(){
	return select;
}

char CFoundBox::GetTType(){
	return tType;
}