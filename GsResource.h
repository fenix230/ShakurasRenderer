#pragma once
#include "UtilityAndMath.h"
#include "GsPrimitive.h"
#include "GsTexture.h"
#include <vector>


SHAKURAS_BEGIN;


//primitive��Դ
void GenerateCube(std::vector<GsTriangle>& prims);


//texture��Դ
GsTextureU32Ptr GenerateGrid();


SHAKURAS_END;