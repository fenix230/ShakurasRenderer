#pragma once
#include "SoftColorFormat.h"
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


template<class CF>
class SoftSurface {
public:
	typedef CF format_t;
	typedef typename CF::data_t data_t;
	typedef typename CF::scalar_t scalar_t;

public:
	SoftSurface() {
		width_ = height_ = 0;
	}

public:
	void reset(int ww, int hh) {
		width_ = ww;
		height_ = hh;

		data_.clear();
		data_.resize(width_ * height_, 0);
	}

	template<typename C>
	void reset(int ww, int hh, const data_t* data, C conv) {
		reset(ww, hh);

		for (int j = 0; j < height_; ++j) {
			for (int i = 0; i < width_; ++i) {
				data_[j * width_ + i] = conv(data[j * width_ + i]);
			}
		}
	}

	inline int width() const { return width_; }
	inline int height() const { return height_; }

	inline data_t getd(int x, int y) const { return data_[y * width_ + x]; }
	inline void setd(int x, int y, const data_t& c) { data_[y * width_ + x] = c; }
	inline scalar_t gets(int x, int y) const { return CF::scalar(data_[y * width_ + x]); }
	inline void sets(int x, int y, const scalar_t& c) { data_[y * width_ + x] = CF::data(c); }

	inline void* buffer() { return (void*)data_.data(); }

private:
	std::vector<data_t> data_;
	int width_, height_;
};


typedef SoftSurface<ColorFormatU32F3> SoftSurfaceU32F3;
SHAKURAS_SHARED_PTR(SoftSurfaceU32F3);


//for SoftSurface reset
inline uint32_t Assign(uint32_t d) {
	return d;
}
inline uint32_t Swap02(uint32_t d) {
	uint8_t* p8 = (uint8_t*)(&d);
	std::swap(p8[0], p8[2]);
	return d;
}


template<class CF, typename AF>
typename CF::scalar_t NearestSample(float u, float v, const SoftSurface<CF>& surface, AF addressing) {
	addressing(u, v);

	u *= (surface.width() - 1);
	v *= (surface.height() - 1);

	int x = (int)(u + 0.5f);
	int y = (int)(v + 0.5f);

	return surface.get(x, y);
}


template<class CF, typename AF>
typename CF::scalar_t BilinearSample(float u, float v, const SoftSurface<CF>& surface, AF addressing) {
	addressing(u, v);

	u *= (surface.width() - 1);
	v *= (surface.height() - 1);

	int fx = (int)u, fy = (int)v;

	int cx = fx + 1, cy = fy + 1;
	int cx_mod = cx % surface.width();
	int cy_mod = cy % surface.height();

	typedef typename CF::scalar_t scalar_t;

	scalar_t lbc = surface.gets(fx, fy);//左下
	scalar_t rbc = surface.gets(cx_mod, fy);//右下
	scalar_t ltc = surface.gets(fx, cy_mod);//左上
	scalar_t rtc = surface.gets(cx_mod, cy_mod);//右上

	//u方向插值
	float it = (cx != fx ? (u - fx) / (cx - fx) : 0.0f);
	scalar_t ibc = lbc + (rbc - lbc) * it;
	scalar_t itc = ltc + (rtc - ltc) * it;

	//v方向插值
	it = (cy != fy ? (v - fy) / (cy - fy) : 0.0f);
	scalar_t ic = ibc + (itc - ibc) * it;

	return ic;
}


SHAKURAS_END;