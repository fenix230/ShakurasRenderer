#pragma once
#include "ResUtility.h"


//texture×ÊÔ´
RESPARSER_DLL void* GridTexture(int& width, int& height, uint32_t c1 = 0xffffff, uint32_t c2 = 0x000000);
RESPARSER_DLL void* LoadTexture(std::string filepath, bool isrelpath, int& width, int& height);