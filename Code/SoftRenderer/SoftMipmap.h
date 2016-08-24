#pragma once
#include "Core\MathAndGeometry.h"
#include "SoftSurface.h"
#include <vector>


SHAKURAS_BEGIN;


template<class CF>
class SoftMipmap {
public:
	typedef CF format_t;
	typedef typename CF::data_t data_t;
	typedef typename CF::scalar_t scalar_t;
	typedef SoftSurface<CF> surface_t;

public:
	SoftMipmap() {}

public:
	void reset(std::shared_ptr<surface_t> surface) {
		surfaces_.clear();

		if (!surface) {
			return;
		}

		surfaces_.push_back(surface);

		int w = (surface->width() + 1) / 2;
		int h = (surface->height() + 1) / 2;
		while (w > 1 || h > 1) {
			std::shared_ptr<surface_t> next_surface = std::make_shared<surface_t>();
			next_surface->reset(w, h);

			for (int x = 0; x != w; x++) {
				int xx = x * 2;

				for (int y = 0; y != h; y++) {
					int yy = y * 2;

					scalar_t c0 = surface->gets(xx, yy);
					scalar_t c1 = surface->gets((xx + 1) % surface->width(), yy);
					scalar_t c2 = surface->gets(xx, (yy + 1) % surface->height());
					scalar_t c3 = surface->gets((xx + 1) % surface->width(), (yy + 1) % surface->height());

					next_surface->sets(x, y, (c0 + c1 + c2  + c3) * 0.25f);
				}
			}

			surface = next_surface;
			surfaces_.push_back(surface);

			w = (w + 1) / 2;
			h = (h + 1) / 2;
		}
	}

	inline int levelCount() const { return (int)surfaces_.size(); }
	inline const surface_t& level(int l) const  {
		if (l < 0) {
			return *surfaces_.front();
		}
		else if (levelCount() <= l) {
			return *surfaces_.back();
		}
		return *surfaces_[l];
	}

private:
	std::vector<std::shared_ptr<surface_t> > surfaces_;
};


typedef SoftMipmap<ColorFormatU32F3> SoftMipmapU32F3;
SHAKURAS_SHARED_PTR(SoftMipmapU32F3);


template<class CF>
std::shared_ptr<SoftMipmap<CF> > CreateSoftMipmap(std::shared_ptr<SoftSurface<CF> > surface) {
	if (!surface) { 
		return nullptr;
	}

	std::shared_ptr<SoftMipmap<CF> > mipmap = std::make_shared<SoftMipmap<CF> >();
	mipmap->reset(surface);

	return mipmap;
}


template<class CF>
float ComputeLevel(const Vector2f& ddx, const Vector2f& ddy, const SoftMipmap<CF>& mipmap) {
	int w = mipmap.level(0).width();
	int h = mipmap.level(0).height();

	Vector2f ddx_ts(ddx.x * w, ddx.y * h);
	Vector2f ddy_ts(ddy.x * w, ddy.y * h);

	float ddx_rho = Length2(ddx_ts);
	float ddy_rho = Length2(ddy_ts);

	float rho = (std::max)(ddx_rho, ddy_rho);

	if (rho == 0.0f) {
		rho = 0.000001f;
	}
	float lambda = log2(rho);
	return lambda - 1.0f;
}


template<class CF, typename AF>
typename CF::scalar_t NearestSample(float u, float v, const Vector2f& ddx, const Vector2f& ddy, const SoftMipmap<CF>& mipmap, AF addressing) {
	float lv = ComputeLevel(ddx, ddy, mipmap);
	float lvf = floorf(lv);
	lvf = Clamp(lvf, 0.0f, (float)mipmap.levelCount() - 1);
	return NearestSample(u, v, mipmap.level((int)lvf), addressing);
}


template<class CF, typename AF>
typename CF::scalar_t BilinearSample(float u, float v, const Vector2f& ddx, const Vector2f& ddy, const SoftMipmap<CF>& mipmap, AF addressing) {
	float lv = ComputeLevel(ddx, ddy, mipmap);
	float lvf = floorf(lv);

	lvf = Clamp(lvf, 0.0f, (float)mipmap.levelCount() - 1);
	return BilinearSample(u, v, mipmap.level((int)lvf), addressing);
}


template<class CF, typename AF>
typename CF::scalar_t TrilinearSample(float u, float v, const Vector2f& ddx, const Vector2f& ddy, const SoftMipmap<CF>& mipmap, AF addressing) {
	float lv = ComputeLevel(ddx, ddy, mipmap);
	int lvf = (int)lv;
	int lvc = lvf + 1;

	lv = Clamp(lv, 0.0f, (float)mipmap.levelCount() - 1);
	lvf = Clamp(lvf, 0, mipmap.levelCount() - 1);
	lvc = Clamp(lvc, 0, mipmap.levelCount() - 1);

	if (lvf == lvc) {
		return BilinearSample(u, v, mipmap.level(lvf), addressing);;
	}
	
	typedef typename CF::scalar_t scalar_t;
	scalar_t cf = BilinearSample(u, v, mipmap.level(lvf), addressing);
	scalar_t cc = BilinearSample(u, v, mipmap.level(lvc), addressing);

	float t = (lv - lvf) / (lvc - lvf);
	return cf + (cc - cf) * t;
}


SHAKURAS_END;