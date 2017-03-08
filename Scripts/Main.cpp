#include"Suken.h"
#include"MyScene.h"

void suken::Awake(){
	SetClass();
	LoadHandle();
}

void suken::GameLoopEnter(){
	DrawGame();
}

void suken::GameLoopExit(){

}