#ifndef _FFPLAY_H_
#define _FFPLAY_H_
#pragma once

#ifdef FFPLAY_EXPORTS
#define FFPLAY_API __declspec(dllexport)
#else
#define FFPLAY_API __declspec(dllimport)
#endif

extern "C" FFPLAY_API int media_open(const char *filename);
extern "C" FFPLAY_API int media_play(HWND hWnd,RECT rcPos);
extern "C" FFPLAY_API int media_close();

#endif
