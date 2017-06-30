#ifndef _FFPLAY_DLL_H_
#define _FFPLAY_DLL_H_
#pragma once

#ifdef FFPLAY_EXPORTS
#define FFPLAY_API __declspec(dllexport)
#else
#define FFPLAY_API __declspec(dllimport)
#endif

class IPlayerCallback
{
public:
	virtual void Progress(int nTimeline) = 0;
};

class IPlayer
{
public:
	virtual bool Init() = 0;
	virtual bool Stop() = 0;
	virtual void Exit() = 0;
	virtual bool Open(const char *filename) = 0;
	virtual void Play(HWND hWnd,RECT rcPos) = 0;
	virtual void Pause() = 0;
};

extern "C" FFPLAY_API IPlayer* PlayerInit();
extern "C" FFPLAY_API bool PlayerExit(IPlayer* pPlayer);

#endif
