#pragma once
#include "MathAndGeometry.h"
#include "GsVertexFragment.h"
#include "GsTexture.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


//texture��Դ
GsSurfacePtr GridTexture();
GsSurfacePtr LoadTexture(std::string filepath);


SHAKURAS_END;