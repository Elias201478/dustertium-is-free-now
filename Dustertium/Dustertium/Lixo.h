typedef struct IUnknown IUnknown;

#pragma once

//Pragma comments & warnings
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "Kernel32.lib")
#pragma warning(disable : 4996)

//Defines
#define PI 3.1415926535

//Includes
#include <Windows.h>
#include <math.h>
#include <time.h>
#include "System.h"
#include "Colors.h"
#include "Payloads.h"
#include "Bytebeats.h"