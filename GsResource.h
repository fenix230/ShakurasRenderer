#pragma once
#include "MathAndGeometry.h"
#include "GsVertexFragment.h"
#include "GsTexture.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


//primitive��Դ
void GenerateCube(std::vector<GsVertexA4>& verts, std::vector<std::array<int, 3> >& itris);


//texture��Դ
GsTextureU32Ptr GridTexture();
GsTextureU32Ptr LoadTexture(std::string filepath);


SHAKURAS_END;