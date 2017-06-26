#ifndef _FFPLAY_H_
#define _FFPLAY_H_
#pragma once

void FFplayInit();
void FFplayExit();
int FFplayOpen(const char *filename);
int FFplayPlay(HWND hWnd,RECT rcPos);
int FFplayPause();
int FFplayStop();

#endif