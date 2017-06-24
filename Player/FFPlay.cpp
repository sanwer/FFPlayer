#include "stdafx.h"
#include "FFplay.h"

#ifdef LOAD_FFPLAY_DLL
typedef int (*LPFFplayInit)(HWND hWnd,RECT rcPos);
typedef int (*LPFFplayOpen)(const char * filename);
typedef int (*LPFFplayPlay)();
typedef int (*LPFFplayPause)();
typedef int (*LPFFplayStop)();
typedef int (*LPFFplayExit)();

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

int FFplayInit(HMODULE hModule,HWND hWnd,RECT rcPos)
{
	int nRet = 0;
#ifdef LOAD_FFPLAY_DLL

#ifdef _DEBUG
		hFFplay = ::LoadLibrary(_T("FFplay_d.dll"));
#else
		hFFplay = ::LoadLibrary(_T("FFplay.dll"));
#endif
		if( hFFplay != NULL ) {
			pFFplayInit = (LPFFplayInit)::GetProcAddress(hFFplay, "ffplay_init");
			pFFplayOpen = (LPFFplayOpen)::GetProcAddress(hFFplay, "ffplay_open");
			pFFplayPlay = (LPFFplayPlay)::GetProcAddress(hFFplay, "ffplay_play");
			pFFplayPause = (LPFFplayPause)::GetProcAddress(hFFplay, "ffplay_pause");
			pFFplayStop = (LPFFplayStop)::GetProcAddress(hFFplay, "ffplay_stop");
			pFFplayExit = (LPFFplayExit)::GetProcAddress(hFFplay, "ffplay_exit");
		}
		if(pFFplayInit){
			nRet = pFFplayInit(hWnd,rcPos);
		}
#else
	nRet = ffplay_init(hWnd,rcPos);
#endif
	return nRet;
}

int FFplayOpen(const char *filename)
{
	int nRet = 0;
#ifdef LOAD_FFPLAY_DLL
		if(pFFplayOpen){
			nRet = pFFplayOpen(filename);
		}
#else
	nRet = ffplay_open(filename);
#endif
	return nRet;
}

int FFplayPlay()
{
	int nRet = 0;
#ifdef LOAD_FFPLAY_DLL
		if(pFFplayPlay){
			nRet = pFFplayPlay();
		}
#else
	nRet = ffplay_play();
#endif
	return nRet;
}

int FFplayPause()
{
	int nRet = 0;
#ifdef LOAD_FFPLAY_DLL
		if(pFFplayPause){
			nRet = pFFplayPause();
		}
#else
	nRet = ffplay_pause();
#endif
	return nRet;
}

int FFplayStop()
{
	int nRet = 0;
#ifdef LOAD_FFPLAY_DLL
		if(pFFplayStop){
			nRet = pFFplayStop();
		}
#else
	nRet = ffplay_stop();
#endif
	return nRet;
}

int FFplayExit()
{
	int nRet = 0;
#ifdef LOAD_FFPLAY_DLL
		if(pFFplayExit){
			nRet = pFFplayExit();
		}
#else
	nRet = ffplay_exit();
#endif
	return nRet;
}