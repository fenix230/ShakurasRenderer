#pragma once
#include "ResUtility.h"


//texture��Դ
RESPARSER_DLL void* GridTexture(int& width, int& height);
RESPARSER_DLL void* LoadTexture(std::string filepath, bool isrelpath, int& width, int& height);