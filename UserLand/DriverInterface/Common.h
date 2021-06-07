#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dshow.h>

#define VIRTUALCAMERA_WIDTH 1280
#define VIRTUALCAMERA_HEIGHT 720
#define VIRTUALCAMERA_NAME "avshws Source"

#ifdef DRIVERINTERFACE_EXPORTS
#define DRIVERINTERFACE extern "C" __declspec(dllexport)
#else
#define DRIVERINTERFACE extern "C" __declspec(dllimport)
#endif