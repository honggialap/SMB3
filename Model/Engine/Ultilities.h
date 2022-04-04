#pragma once
#ifndef __ULTILITIES_H__
#define __ULTILITIES_H__

#pragma region INCLUDE
#include "pugixml.hpp"
#include <windows.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <codecvt>
#include <locale>
#include <map>
#pragma endregion

void DebugOut(const wchar_t* fmt, ...);

#endif // !__ULTILITIES_H__
