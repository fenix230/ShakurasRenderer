#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


//ÑÕÉ«×ª»»
inline uint32_t Rgbi(const Vector3f& v) {
	uint32_t u = 0;
	unsigned char* pu = (unsigned char*)&u;
	pu[0] = (int)(v.x * 255);
	pu[1] = (int)(v.y * 255);
	pu[2] = (int)(v.z * 255);
	pu[3] = 255;
	return u;
}
inline uint32_t Rgbai(const Vector4f& v) {
	uint32_t u = 0;
	unsigned char* pu = (unsigned char*)&u;
	pu[0] = (int)(v.x * 255);
	pu[1] = (int)(v.y * 255);
	pu[2] = (int)(v.z * 255);
	pu[3] = (int)(v.w * 255);
	return u;
}
inline Vector3f Rgbv(uint32_t u) {
	Vector3f v;
	const unsigned char* pu = (const unsigned char*)&u;
	v.x = pu[0] / 255.0f;
	v.y = pu[1] / 255.0f;
	v.z = pu[2] / 255.0f;
	return v;
}
inline Vector4f Rgbav(uint32_t u) {
	Vector4f v;
	const unsigned char* pu = (const unsigned char*)&u;
	v.x = pu[0] / 255.0f;
	v.y = pu[1] / 255.0f;
	v.z = pu[2] / 255.0f;
	v.w = pu[3] / 255.0f;
	return v;
}


inline uint32_t Bgri(const Vector3f& v) {
	uint32_t u = 0;
	unsigned char* pu = (unsigned char*)&u;
	pu[2] = (int)(v.x * 255);
	pu[1] = (int)(v.y * 255);
	pu[0] = (int)(v.z * 255);
	pu[3] = 255;
	return u;
}
inline uint32_t Bgrai(const Vector4f& v) {
	uint32_t u = 0;
	unsigned char* pu = (unsigned char*)&u;
	pu[2] = (int)(v.x * 255);
	pu[1] = (int)(v.y * 255);
	pu[0] = (int)(v.z * 255);
	pu[3] = (int)(v.w * 255);
	return u;
}
inline Vector3f Bgrv(uint32_t u) {
	Vector3f v;
	const unsigned char* pu = (const unsigned char*)&u;
	v.x = pu[2] / 255.0f;
	v.y = pu[1] / 255.0f;
	v.z = pu[0] / 255.0f;
	return v;
}
inline Vector4f Bgrav(uint32_t u) {
	Vector4f v;
	const unsigned char* pu = (const unsigned char*)&u;
	v.x = pu[2] / 255.0f;
	v.y = pu[1] / 255.0f;
	v.z = pu[0] / 255.0f;
	v.w = pu[3] / 255.0f;
	return v;
}


SHAKURAS_END;