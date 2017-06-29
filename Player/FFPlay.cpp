#include "stdafx.h"
#include "FFplay.h"

#ifdef LOAD_FFPLAY_DLL
typedef bool (*LPFFplayInit)();
typedef bool (*LPFFplayOpen)(const char * filename);
typedef void (*LPFFplayPlay)(HWND hWnd,RECT rcPos);
typedef void (*LPFFplayPause)();
typedef bool (*LPFFplayStop)();
typedef void (*LPFFplayExit)();

HMODULE hFFplay = NULL;
LPFFplayInit pFFplayInit = NULL;
LPFFplayOpen pFFplayOpen = NULL;
LPFFplayPlay pFFplayPlay = NULL;
LPFFplayPause pFFplayPause = NULL;
LPFFplayStop pFFplayStop = NULL;
LPFFplayExit pFFplayExit = NULL;
#else
# include "..\FFPlay\FFPlay.h"
# ifdef _DEBUG
#  pragma comment(lib, "FFPlay_d.lib")
# else
#  pragma comment(lib, "FFPlay.lib")
# endif
#endif

void FFplayInitDLL()
{
#ifdef LOAD_FFPLAY_DLL
#ifdef _DEBUG
	hFFplay = ::LoadLibrary(_T("FFplay_d.dll"));
#else
	hFFplay = ::LoadLibrary(_T("FFplay.dll"));
#endif
	if(hFFplay) {
		pFFplayInit = (LPFFplayInit)::GetProcAddress(hFFplay, "ffplay_init");
		pFFplayOpen = (LPFFplayOpen)::GetProcAddress(hFFplay, "ffplay_open");
		pFFplayPlay = (LPFFplayPlay)::GetProcAddress(hFFplay, "ffplay_play");
		pFFplayPause = (LPFFplayPause)::GetProcAddress(hFFplay, "ffplay_pause");
		pFFplayStop = (LPFFplayStop)::GetProcAddress(hFFplay, "ffplay_stop");
		pFFplayExit = (LPFFplayExit)::GetProcAddress(hFFplay, "ffplay_exit");
	}
#endif
}

void FFplayExitDLL()
{
#ifdef LOAD_FFPLAY_DLL
	if(hFFplay) {
		FreeLibrary(hFFplay);
		hFFplay = NULL;
	}
#endif
}

void FFplayInit()
{
#ifdef LOAD_FFPLAY_DLL
	if(pFFplayInit)
		pFFplayInit();
#else
	ffplay_init();
#endif
}

void FFplayExit()
{
#ifdef LOAD_FFPLAY_DLL
	if(pFFplayExit)
		pFFplayExit();
#else
	ffplay_exit();
#endif
}

bool FFplayOpen(const char *filename)
{
	bool bRet = false;
#ifdef LOAD_FFPLAY_DLL
	if(pFFplayOpen){
		bRet = pFFplayOpen(filename);
	}
#else
	bRet = ffplay_open(filename);
#endif
	return bRet;
}

void FFplayPlay(HWND hWnd,RECT rcPos)
{
#ifdef LOAD_FFPLAY_DLL
	if(pFFplayPlay){
		pFFplayPlay(hWnd,rcPos);
	}
#else
	ffplay_play(hWnd,rcPos);
#endif
}

void FFplayPause()
{
#ifdef LOAD_FFPLAY_DLL
	if(pFFplayPause){
		pFFplayPause();
	}
#else
	ffplay_pause();
#endif
}

bool FFplayStop()
{
	bool bRet = false;
#ifdef LOAD_FFPLAY_DLL
	if(pFFplayStop){
		bRet = pFFplayStop();
	}
#else
	bRet = ffplay_stop() == 0;
#endif
	return bRet;
}
