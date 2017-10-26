#include"Suken.h"
#include"MyScene.h"
#include"Music.h"

void suken::Awake(){
	SetClass();
	AwakeMusic();
}

void suken::GameLoopEnter(){
	DrawGame();
}

void suken::GameLoopExit(){
	music->Play();
}