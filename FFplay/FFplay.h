#ifndef _FFPLAY_DLL_H_
#define _FFPLAY_DLL_H_
#pragma once

#ifdef FFPLAY_EXPORTS
#define FFPLAY_API __declspec(dllexport)
#else
#define FFPLAY_API __declspec(dllimport)
#endif

extern "C" FFPLAY_API int ffplay_init(HWND hWnd,RECT rcPos);
extern "C" FFPLAY_API int ffplay_open(const char *filename);
extern "C" FFPLAY_API int ffplay_play();
extern "C" FFPLAY_API int ffplay_pause();
extern "C" FFPLAY_API int ffplay_stop();
extern "C" FFPLAY_API int ffplay_exit();

#endif
