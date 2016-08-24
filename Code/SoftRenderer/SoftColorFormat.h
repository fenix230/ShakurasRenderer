#pragma once
#include "Core/MathAndGeometry.h"


SHAKURAS_BEGIN;


//uint32_t - b, g, r
//Vector3f - r, g, b
class ColorFormatU32F3 {
public:
	typedef uint32_t data_t;
	typedef Vector3f scalar_t;

	static inline Vector3f scalar(uint32_t d) {
		Vector3f v;
		const unsigned char* pu = (const unsigned char*)&d;
		v.x = pu[2] / 255.0f;
		v.y = pu[1] / 255.0f;
		v.z = pu[0] / 255.0f;
		return v;
	}

	static inline uint32_t data(const Vector3f& v) {
		uint32_t u = 0;
		unsigned char* pu = (unsigned char*)&u;
		pu[2] = (int)(v.x * 255);
		pu[1] = (int)(v.y * 255);
		pu[0] = (int)(v.z * 255);
		pu[3] = 255;
		return u;
	}

	static inline Vector3f clean() {
		return Vector3f(0.2f, 0.2f, 0.6f);
	}
};


SHAKURAS_END;