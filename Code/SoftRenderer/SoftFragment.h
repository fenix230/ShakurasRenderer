#pragma once
#include "Core/MathAndGeometry.h"


SHAKURAS_BEGIN;


template<class V, class C>
class SoftFragment {
public:
	SoftFragment() : x(0), y(0), z(1.0f), weight(0.0f) {}

public:
	int x, y;
	float z;
	V varyings;
	float weight;
	C c;
};


template<class V>
inline Vector2f TexCoord(const V& vl) {
	return Vector2f();
}


SHAKURAS_END;