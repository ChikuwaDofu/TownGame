#include"TileM.h"

void CTileManager::Set(){
	for(int y=0; y<BLOCKS_Y; y++){
		for(int x=0; x<BLOCKS_X; x++){
			tile[x+y*BLOCKS_X].Set(x*GRID+300, y*GRID, PLAIN);
		}
	}

	tile[0].Set(300, 0, SEA);

	info = false;
	infoNum = -1;
}

void CTileManager::CheckInfo(){
	if (!info){
		infoNum = -1;
		for (int i = 0; i < 100; i++){
			if (tile[i].GetInfo()){
				info = true;
				infoNum = i;
				break;
			}
		}

		if (info){
			if (tile[infoNum].GetTown() == -1){
				fbox = new CFoundBox();
			}
		}
	}
	else{
		if (fbox->GetClose()){
			delete fbox;
			info = false;
		}

		if (fbox->GetSelect()){
			tile[infoNum].SetTown(fbox->GetTType());
			delete fbox;
			info = false;
		}
	}
}

void CTileManager::Draw(){
	CheckInfo();

	if(info){
		fbox->DrawFB();
	}else{
		for(int i=0; i<100; i++){
			tile[i].Draw();
		}
	}
}