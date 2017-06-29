#ifndef _FFPLAY_DLL_H_
#define _FFPLAY_DLL_H_
#pragma once

#ifdef FFPLAY_EXPORTS
#define FFPLAY_API __declspec(dllexport)
#else
#define FFPLAY_API __declspec(dllimport)
#endif

extern "C" FFPLAY_API bool ffplay_init();
extern "C" FFPLAY_API bool ffplay_open(const char *filename);
extern "C" FFPLAY_API void ffplay_play(HWND hWnd,RECT rcPos);
extern "C" FFPLAY_API void ffplay_pause();
extern "C" FFPLAY_API bool ffplay_stop();
extern "C" FFPLAY_API void ffplay_exit();

class IPlayerCallback
{
public:
	virtual void Progress(int nTimeline) = 0;
};

class IPlayer
{
public:
	virtual void Init() = 0;
	virtual bool Stop() = 0;
	virtual void Exit() = 0;
	virtual bool Open(const char *filename) = 0;
	virtual void Play(HWND hWnd,RECT rcPos) = 0;
	virtual void Pause() = 0;
};

class FFPLAY_API CFFplay : public IPlayer
{
public:
	CFFplay();
	virtual ~CFFplay();

	void Init();
	bool Stop();
	void Exit();
	bool Open(const char *filename);
	void Play(HWND hWnd,RECT rcPos);
	void Pause();

private:
	IPlayerCallback* m_pCallback;
	void* pData;
};

extern "C" FFPLAY_API IPlayer* FFplayInit();
extern "C" FFPLAY_API bool FFplayExit(IPlayer* pPlayer);

#endif
