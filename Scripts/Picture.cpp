#include"Picture.h"

CPicture::CPicture(const char* file){
	int buf;
	buf = LoadGraph(file, true);
	handle.push_back(buf);
}

CPicture::CPicture(const char* file, int xNum, int yNum, int xSize, int ySize, int num){
	int* buf;
	buf = new int [num];
	LoadDivGraph(file,num,xNum,yNum,xSize,ySize,buf);
	for(int i=0;i<num;i++){
		handle.push_back(buf[i]);
	}
	delete[] buf;
}

void CPicture::Draw(int x, int y){
	DrawGraph(x, y, handle[0], true);
}

void CPicture::Draw(int x, int y, int num){
	DrawGraph(x, y, handle[num], true);
}

void CPicture::Load(const char* file){
	int buf;
	buf = LoadGraph(file, true);
	handle.push_back(buf);
}

void CPicture::Load(const char* file, int xNum, int yNum, int xSize, int ySize, int num){
	int* buf;
	buf = new int [num];
	LoadDivGraph(file,num,xNum,yNum,xSize,ySize,buf);
	for(int i=0;i<num;i++){
		handle.push_back(buf[i]);
	}
	delete[] buf;
}