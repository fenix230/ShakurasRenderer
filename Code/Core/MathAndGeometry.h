#pragma once
#include "Utility.h"
#include <array>
#include <memory>
#include <stdint.h>


SHAKURAS_BEGIN;


//向量
template<class S>
class Vector2 {
public:
	S x, y;

public:
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(S xx, S yy) : x(xx), y(yy) {}

public:
	void set(S xx, S yy) {
		x = xx;
		y = yy;
	}

	const S* data() const {
		return &x;
	}
};

template<class S>
inline Vector2<S> operator+(const Vector2<S>& v1, const Vector2<S>& v2) {
	return Vector2<S>(v1.x + v2.x, v1.y + v2.y);
}
template<class S>
inline Vector2<S> operator-(const Vector2<S>& v1, const Vector2<S>& v2) {
	return Vector2<S>(v1.x - v2.x, v1.y - v2.y);
}
template<class S>
inline Vector2<S> operator*(const Vector2<S>& v1, S t) {
	return Vector2<S>(v1.x * t, v1.y * t);
}
template<class S>
inline Vector2<S> operator/(const Vector2<S>& v1, S d) {
	S t = 1.0f / d;
	return Vector2<S>(v1.x * t, v1.y * t);
}


template<class S> class Vector4;


template<class S>
class Vector3 {
public:
	S x, y, z;

public:
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(S xx, S yy, S zz) : x(xx), y(yy), z(zz) {}
	
public:
	void set(S xx, S yy, S zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	Vector2<S> xy() const {
		return Vector2<S>(x, y);
	}

	Vector4<S> xyzw(S w) const {
		return Vector4<S>(x, y, z, w);
	}

	const S* data() const {
		return &x;
	}
};

template<class S>
inline Vector3<S> operator+(const Vector3<S>& v1, const Vector3<S>& v2) {
	return Vector3<S>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
template<class S>
inline Vector3<S> operator-(const Vector3<S>& v1, const Vector3<S>& v2) {
	return Vector3<S>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
template<class S>
inline Vector3<S> operator*(const Vector3<S>& v1, S t) {
	return Vector3<S>(v1.x * t, v1.y * t, v1.z * t);
}
template<class S>
inline Vector3<S> operator/(const Vector3<S>& v1, S d) {
	S t = 1.0f / d;
	return Vector3<S>(v1.x * t, v1.y * t, v1.z * t);
}


//齐次向量
template<class S>
class Vector4 {
public:
	S x, y, z, w;

public:
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(S xx, S yy, S zz, S ww) : x(xx), y(yy), z(zz), w(ww) {}

public:
	void set(S xx, S yy, S zz, S ww = 0.0f) {
		x = xx;
		y = yy;
		z = zz;
		w = ww;
	}

	Vector2<S> xy() const {
		return Vector2<S>(x, y);
	}

	Vector3<S> xyz() const {
		return Vector3<S>(x, y, z);
	}

	const S* data() const {
		return &x;
	}
};

template<class S>
inline Vector4<S> operator+(const Vector4<S>& v1, const Vector4<S>& v2) {
	return Vector4<S>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}
template<class S>
inline Vector4<S> operator-(const Vector4<S>& v1, const Vector4<S>& v2) {
	return Vector4<S>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}
template<class S>
inline Vector4<S> operator*(const Vector4<S>& v1, S t) {
	return Vector4<S>(v1.x * t, v1.y * t, v1.z * t, v1.w * t);
}
template<class S>
inline Vector4<S> operator*(const Vector4<S>& v1, const Vector4<S>& v2) {
	return Vector4<S>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}
template<class S>
inline Vector4<S> operator/(const Vector4<S>& v1, S d) {
	S t = 1.0f / d;
	return Vector4<S>(v1.x * t, v1.y * t, v1.z * t, v1.w * t);
}


template<class V2>
inline float Length2(const V2& v) {
	return ::sqrt(v.x * v.x + v.y * v.y);
}

template<class V3>
inline float Length3(const V3& v) {
	return ::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template<class V2>
inline void Normalize2(V2& v, float len = 1.0f) {
	float cur_len = Length2(v);
	if (cur_len != len) {
		float scale = len / cur_len;
		v.x *= scale;
		v.y *= scale;
	}
}

template<class V3>
inline void Normalize3(V3& v, float len = 1.0f) {
	float cur_len = Length3(v);
	if (cur_len != len) {
		float scale = len / cur_len;
		v.x *= scale;
		v.y *= scale;
		v.z *= scale;
	}
}

template<class V3>
inline float DotProduct3(const V3& v1, const V3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<class V4>
inline float DotProduct4(const V4& v1, const V4& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template<class V2>
float CrossProduct2(const V2& v1, const V2& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

template<class V3>
inline V3 CrossProduct3(const V3& v1, const V3& v2) {
	V3 v3;
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
	return v3;
}

template<class V3>
inline V3 Reflect(const V3& i, const V3& n) {
	return i - n * (2.0f * DotProduct3(i, n));
}


//矩阵
template<class S>
class Matrix33 {
public:
	Matrix33() {
		reset();
	}

public:
	Matrix33<S>& zero() {
		m[0][0] = m[0][1] = m[0][2] = 0.0f;
		m[1][0] = m[1][1] = m[1][2] = 0.0f;
		m[2][0] = m[2][1] = m[2][2] = 0.0f;
		return *this;
	}

	Matrix33<S>& reset() {
		m[0][0] = m[1][1] = m[2][2] = 1.0f;
		m[0][1] = m[0][2] = 0.0f;
		m[1][0] = m[1][2] = 0.0f;
		m[2][0] = m[2][1] = 0.0f;
		return *this;
	}

	Vector3<S> transform(const Vector3<S>& v1) const {
		Vector3<S> v2;
		v2.x = v1.x * m[0][0] + v1.y * m[1][0] + v1.z * m[2][0];
		v2.y = v1.x * m[0][1] + v1.y * m[1][1] + v1.z * m[2][1];
		v2.z = v1.x * m[0][2] + v1.y * m[1][2] + v1.z * m[2][2];
		return v2;
	}

	std::array<S, 9> flat() const {
		std::array<S, 9> res = {
			m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2]
		};
		return res;
	}

	static Matrix33<S> Scale(S x, S y, S z) {
		Matrix33<S> mat;
		mat.m[0][0] = x;
		mat.m[1][1] = y;
		mat.m[2][2] = z;
		return mat;
	}

public:
	std::array<std::array<S, 3>, 3> m;
};

template<class S>
inline Matrix33<S> operator+(const Matrix33<S>& m1, const Matrix33<S>& m2) {
	Matrix33<S> m3;
	for (size_t i = 0; i != 3; i++) {
		for (size_t ii = 0; ii != 3; ii++) {
			m3.m[i][ii] = m1.m[i][ii] + m2.m[i][ii];
		}
	}
	return m3;
}
template<class S>
inline Matrix33<S> operator-(const Matrix33<S>& m1, const Matrix33<S>& m2) {
	Matrix33<S> m3;
	for (int i = 0; i != 3; i++) {
		for (int ii = 0; ii != 3; ii++) {
			m3.m[i][ii] = m1.m[i][ii] - m2.m[i][ii];
		}
	}
	return m3;
}
template<class S>
inline Matrix33<S> operator*(const Matrix33<S>& m1, const Matrix33<S>& m2) {
	Matrix33<S> m3;
	for (size_t i = 0; i != 3; i++) {
		for (size_t ii = 0; ii != 3; ii++) {
			m3.m[ii][i] = m1.m[ii][0] * m2.m[0][i] +
				m1.m[ii][1] * m2.m[1][i] +
				m1.m[ii][2] * m2.m[2][i];
		}
	}
	return m3;
}
template<class S>
inline Matrix33<S> operator*(const Matrix33<S>& m1, S t) {
	Matrix33<S> m2;
	for (int i = 0; i != 3; i++) {
		for (int ii = 0; ii != 3; ii++) {
			m2.m[i][ii] = m1[i][ii] * t;
		}
	}
	return m2;
}


template<class S>
class Matrix44 {
public:
	Matrix44() {
		reset();
	}

public:
	Matrix44<S>& zero() {
		m[0][0] = m[0][1] = m[0][2] = m[0][3] = 0.0f;
		m[1][0] = m[1][1] = m[1][2] = m[1][3] = 0.0f;
		m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
		m[3][0] = m[3][1] = m[3][2] = m[3][3] = 0.0f;
		return *this;
	}

	Matrix44<S>& reset() {
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
		m[0][1] = m[0][2] = m[0][3] = 0.0f;
		m[1][0] = m[1][2] = m[1][3] = 0.0f;
		m[2][0] = m[2][1] = m[2][3] = 0.0f;
		m[3][0] = m[3][1] = m[3][2] = 0.0f;
		return *this;
	}

	Vector4<S> transform(const Vector4<S>& v1) const {
		Vector4<S> v2;
		v2.x = v1.x * m[0][0] + v1.y * m[1][0] + v1.z * m[2][0] + v1.w * m[3][0];
		v2.y = v1.x * m[0][1] + v1.y * m[1][1] + v1.z * m[2][1] + v1.w * m[3][1];
		v2.z = v1.x * m[0][2] + v1.y * m[1][2] + v1.z * m[2][2] + v1.w * m[3][2];
		v2.w = v1.x * m[0][3] + v1.y * m[1][3] + v1.z * m[2][3] + v1.w * m[3][3];
		return v2;
	}

	std::array<S, 16> flat() const {
		std::array<S, 16> res = {
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]
		};
		return res;
	}

	Matrix33<S> mat3() const {
		Matrix33<S> m3;
		std::copy_n(m[0].begin(), 3, m3.m[0].begin());
		std::copy_n(m[1].begin(), 3, m3.m[1].begin());
		std::copy_n(m[2].begin(), 3, m3.m[2].begin());
		return m3;
	}

	static Matrix44<S> Translate(S x, S y, S z) {
		Matrix44<S> mat;
		mat.m[3][0] = x;
		mat.m[3][1] = y;
		mat.m[3][2] = z;
		return mat;
	}

	static Matrix44<S> Scale(S x, S y, S z) {
		Matrix44<S> mat;
		mat.m[0][0] = x;
		mat.m[1][1] = y;
		mat.m[2][2] = z;
		return mat;
	}

	static Matrix44<S> Rotate(S x, S y, S z, S theta) {
		Matrix44<S> mat;
		S qsin = (S)sin(theta * 0.5f);
		S qcos = (S)cos(theta * 0.5f);
		Vector3<S> vec(x, y, z);
		S w = qcos;
		Normalize3(vec);
		x = vec.x * qsin;
		y = vec.y * qsin;
		z = vec.z * qsin;
		mat.m[0][0] = 1 - 2 * y * y - 2 * z * z;
		mat.m[1][0] = 2 * x * y - 2 * w * z;
		mat.m[2][0] = 2 * x * z + 2 * w * y;
		mat.m[0][1] = 2 * x * y + 2 * w * z;
		mat.m[1][1] = 1 - 2 * x * x - 2 * z * z;
		mat.m[2][1] = 2 * y * z - 2 * w * x;
		mat.m[0][2] = 2 * x * z - 2 * w * y;
		mat.m[1][2] = 2 * y * z + 2 * w * x;
		mat.m[2][2] = 1 - 2 * x * x - 2 * y * y;
		mat.m[0][3] = mat.m[1][3] = mat.m[2][3] = 0.0f;
		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.0f;
		mat.m[3][3] = 1.0f;
		return mat;
	}

	static Matrix44<S> LookAt(const Vector3<S>& eye, const Vector3<S>& at, const Vector3<S>& up) {
		Vector3<S> xaxis, yaxis, zaxis;
		zaxis = at - eye;
		Normalize3(zaxis);
		xaxis = CrossProduct3(up, zaxis);
		Normalize3(xaxis);
		yaxis = CrossProduct3(zaxis, xaxis);

		Matrix44<S> mat;
		mat.m[0][0] = xaxis.x;
		mat.m[1][0] = xaxis.y;
		mat.m[2][0] = xaxis.z;
		mat.m[3][0] = -DotProduct3(xaxis, eye);

		mat.m[0][1] = yaxis.x;
		mat.m[1][1] = yaxis.y;
		mat.m[2][1] = yaxis.z;
		mat.m[3][1] = -DotProduct3(yaxis, eye);

		mat.m[0][2] = zaxis.x;
		mat.m[1][2] = zaxis.y;
		mat.m[2][2] = zaxis.z;
		mat.m[3][2] = -DotProduct3(zaxis, eye);

		mat.m[0][3] = mat.m[1][3] = mat.m[2][3] = 0.0f;
		mat.m[3][3] = 1.0f;

		return mat;
	}

	static Matrix44<S> Perspective(S fovy, S aspect, S zn, S zf) {
		Matrix44<S> mat;
		mat.zero();

		S fax = 1.0f / (S)tan(fovy * 0.5f);
		mat.m[0][0] = (S)(fax / aspect);
		mat.m[1][1] = (S)(fax);
		mat.m[2][2] = zf / (zf - zn);
		mat.m[3][2] = -zn * zf / (zf - zn);
		mat.m[2][3] = 1;

		return mat;
	}

public:
	std::array<std::array<S, 4>, 4> m;
};


template<class S>
inline Matrix44<S> operator+(const Matrix44<S>& m1, const Matrix44<S>& m2) {
	Matrix44<S> m3;
	for (size_t i = 0; i != 4; i++) {
		for (size_t ii = 0; ii != 4; ii++) {
			m3.m[i][ii] = m1.m[i][ii] + m2.m[i][ii];
		}
	}
	return m3;
}
template<class S>
inline Matrix44<S> operator-(const Matrix44<S>& m1, const Matrix44<S>& m2) {
	Matrix44<S> m3;
	for (int i = 0; i != 4; i++) {
		for (int ii = 0; ii != 4; ii++) {
			m3.m[i][ii] = m1.m[i][ii] - m2.m[i][ii];
		}
	}
	return m3;
}
template<class S>
inline Matrix44<S> operator*(const Matrix44<S>& m1, const Matrix44<S>& m2) {
	Matrix44<S> m3;
	for (size_t i = 0; i != 4; i++) {
		for (size_t ii = 0; ii != 4; ii++) {
			m3.m[ii][i] = m1.m[ii][0] * m2.m[0][i] +
				m1.m[ii][1] * m2.m[1][i] +
				m1.m[ii][2] * m2.m[2][i] +
				m1.m[ii][3] * m2.m[3][i];
		}
	}
	return m3;
}
template<class S>
inline Matrix44<S> operator*(const Matrix44<S>& m1, S t) {
	Matrix44<S> m2;
	for (int i = 0; i != 4; i++) {
		for (int ii = 0; ii != 4; ii++) {
			m2.m[i][ii] = m1[i][ii] * t;
		}
	}
	return m2;
}


typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;
typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;
typedef Matrix44<float> Matrix44f;
typedef Matrix33<float> Matrix33f;


#define kGSPI 3.1415926f


template<class T>
inline T Clamp(T val, T low, T high) {
	return val < low ? low : (high < val ? high : val);
}


template<class S>
inline S Lerp(S v1, S v2, float t) {
	return v1 + (v2 - v1) * t;
}


SHAKURAS_END;