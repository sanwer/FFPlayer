#ifndef _FFPLAY_H_
#define _FFPLAY_H_
#pragma once

void FFplayInitDLL();
void FFplayExitDLL();
void FFplayInit();
void FFplayExit();
bool FFplayOpen(const char *filename);
void FFplayPlay(HWND hWnd,RECT rcPos);
void FFplayPause();
bool FFplayStop();

#endif