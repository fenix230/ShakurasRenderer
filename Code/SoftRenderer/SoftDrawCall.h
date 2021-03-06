#pragma once
#include "SoftPrimitiveList.h"
#include "Core/MathAndGeometry.h"
#include <vector>


SHAKURAS_BEGIN;


template<class UL, class A, class V>
struct SoftDrawCall {
	UL uniforms;
	SoftPrimitiveList<A, V> prims;
};


SHAKURAS_END;