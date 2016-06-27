#pragma once
#include "MathAndGeometry.h"
#include "GsPrimitive.h"
#include "GsTexture.h"
#include <vector>


SHAKURAS_BEGIN;


//primitive��Դ
void GenerateCube(std::vector<GsTriangle>& prims);


//texture��Դ
GsTextureU32Ptr GridTexture();
GsTextureU32Ptr LoadTexture(std::string filepath);


SHAKURAS_END;