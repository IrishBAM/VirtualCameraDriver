#pragma once

#include "Common.h"

#include <string>

int EnumerateDevicePaths(std::string* paths, int maxCount);
int GetFilter(std::string path, IBaseFilter** filter);