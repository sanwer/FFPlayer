#ifndef _FFPLAY_H_
#define _FFPLAY_H_
#pragma once

int FFplayInit(HMODULE hModule,HWND hWnd,RECT rcPos);
int FFplayOpen(const char *filename);
int FFplayPlay();
int FFplayPause();
int FFplayStop();
int FFplayExit();

#endif