#pragma once
#include "SoftSurface.h"
#include "SoftMipmap.h"
#include "Core/MathAndGeometry.h"


SHAKURAS_BEGIN;


class SoftSampler {
public:
	template<class CF, typename AF>
	typename CF::scalar_t surfaceNearest(float u, float v, const SoftSurface<CF>& surface, AF addressing) {
		return NearestSample(u, v, surface, addressing);
	}

	template<class CF, typename AF>
	typename CF::scalar_t surfaceBilinear(float u, float v, const SoftSurface<CF>& surface, AF addressing) {
		return BilinearSample(u, v, surface, addressing);
	}

	template<class CF, typename AF>
	typename CF::scalar_t mipmapNearest(float u, float v, const SoftMipmap<CF>& mipmap, AF addressing) {
		return NearestSample(u, v, ddx_, ddy_, mipmap, addressing);
	}

	template<class CF, typename AF>
	typename CF::scalar_t mipmapBilinear(float u, float v, const SoftMipmap<CF>& mipmap, AF addressing) {
		return BilinearSample(u, v, ddx_, ddy_, mipmap, addressing);
	}

	template<class CF, typename AF>
	typename CF::scalar_t mipmapTrilinear(float u, float v, const SoftMipmap<CF>& mipmap, AF addressing) {
		return TrilinearSample(u, v, ddx_, ddy_, mipmap, addressing);
	}

public:
	Vector2f ddx_, ddy_;//uvµÄµ¼Êý
};


SHAKURAS_END;