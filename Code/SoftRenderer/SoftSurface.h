#pragma once
#include "Core/MathAndGeometry.h"
#include "Core/Color.h"
#include <vector>
#include <math.h>
#include <assert.h>


SHAKURAS_BEGIN;


inline void ClampAddr(float& u, float& v) {
	u = Clamp(u, 0.0f, 1.0f);
	v = Clamp(v, 0.0f, 1.0f);
}

inline void RepeatAddr(float& u, float& v) {
	u = fmodf(u, 1.0f);
	v = fmodf(v, 1.0f);

	u = (u < 0.0f ? 1.0f + u : u);
	v = (v < 0.0f ? 1.0f + v : v);

	ClampAddr(u, v);
}


class SoftSurface {
public:
	SoftSurface() {}

public:
	void reset(int ww, int hh, const uint32_t* data) {
		width_ = ww;
		height_ = hh;

		data_.clear();
		data_.resize(height_, std::vector<uint32_t>(width_, 0));

		if (data) {
			for (int j = 0; j < height_; ++j) {
				for (int i = 0; i < width_; ++i) {
					uint32_t* c = &data_[j][i];
					*c = data[j*width_ + i];
					uint8_t* rgba = (uint8_t*)c;
					std::swap(rgba[0], rgba[2]);
				}
			}
		}
	}

	inline int width() const { return width_; }
	inline int height() const { return height_; }
	inline Vector3f get(int x, int y) const { return Rgbv(data_[y][x]); }
	inline void set(int x, int y, const Vector3f& c) { data_[y][x] = Rgbi(c); }

private:
	std::vector<std::vector<uint32_t> > data_;
	int width_, height_;
};


SHAKURAS_SHARED_PTR(SoftSurface);


template<class S, typename AF>
Vector3f NearestSample(float u, float v, const S& surface, AF addressing) {
	addressing(u, v);

	u *= (surface.width() - 1);
	v *= (surface.height() - 1);

	int x = (int)(u + 0.5f);
	int y = (int)(v + 0.5f);

	return surface.get(x, y);
}


template<class S, typename AF>
Vector3f BilinearSample(float u, float v, const S& surface, AF addressing) {
	addressing(u, v);

	u *= (surface.width() - 1);
	v *= (surface.height() - 1);

	int fx = (int)u, fy = (int)v;

	int cx = fx + 1, cy = fy + 1;
	int cx_mod = cx % surface.width();
	int cy_mod = cy % surface.height();

	Vector3f lbc = surface.get(fx, fy);//左下
	Vector3f rbc = surface.get(cx_mod, fy);//右下
	Vector3f ltc = surface.get(fx, cy_mod);//左上
	Vector3f rtc = surface.get(cx_mod, cy_mod);//右上

	//u方向插值
	float it = (cx != fx ? (u - fx) / (cx - fx) : 0.0f);
	Vector3f ibc = lbc + (rbc - lbc) * it;
	Vector3f itc = ltc + (rtc - ltc) * it;

	//v方向插值
	it = (cy != fy ? (v - fy) / (cy - fy) : 0.0f);
	Vector3f ic = ibc + (itc - ibc) * it;

	return ic;
}


SHAKURAS_END;