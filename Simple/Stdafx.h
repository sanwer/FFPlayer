#ifndef _STDAFX_H_
#define _STDAFX_H_
#pragma once
#pragma warning (disable:4013 4018 4244 4819 4996)

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#define WIN32_LEAN_AND_MEAN

#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>
#include <time.h>
#include <tchar.h>

#include <vld.h>
#pragma comment( lib, "vld.lib" )

#include "resource.h"

#include "..\..\DuiLib\UIlib.h"
using namespace DuiLib;

#include "..\Player\Player.h"

#endif