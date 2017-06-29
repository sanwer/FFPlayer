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

#endif
