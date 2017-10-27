#pragma once

#include"Suken.h"

enum Sounds {
	sFound,
	sDev,
	sBuild,
	sTDemo,
	sBDemo,
	sSABuild,
	sCut,
	sSerSuc,
	sSerFail,
	sTool,
	sChar,
	sEndUse,
	sBuy,
	sbTitle,
	sbNormal,
	sbCastle,
	sbAgri,
	sbWood,
	sbStone,
	sbComm,
	sbPasture,
	sbChar,
	sbFactory,
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
