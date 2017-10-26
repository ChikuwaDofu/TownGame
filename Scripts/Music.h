#pragma once

#include"Suken.h"

enum Sounds {
	sFound,
	sDev,
	sBuild,
	sBuy,
	sEnd
};

class CMusic{
private:
	int sound[sEnd];
	bool sFlag[sEnd];
	bool loopFlag[sEnd];

public:
	CMusic();
	void AddSound(Sounds sound);
	void Loop(Sounds sound);
	void StopLoop(Sounds sound);
	void Play();
	friend void AwakeMusic();
};

extern CMusic *music;
