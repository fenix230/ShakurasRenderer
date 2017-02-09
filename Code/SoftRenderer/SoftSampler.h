#pragma once
#include "SoftSurface.h"
#include "SoftMipmap.h"
#include "Core/MathAndGeometry.h"


SHAKURAS_BEGIN;

class SoftSampler {
public:
	template<class CF, typename AF>
	typename CF::scalar_t surfaceNearest(float u, float v, const SoftMipmap<CF>& mipmap, AF addressing) {
		return NearestSample(u, v, mipmap.level(0), addressing);
	}

	template<class CF, typename AF>
	typename CF::scalar_t surfaceBilinear(float u, float v, const SoftMipmap<CF>& mipmap, AF addressing) {
		return BilinearSample(u, v, mipmap.level(0), addressing);
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

	template<class CF, typename AF>
	typename CF::scalar_t mipmapAniso(float u, float v, const SoftMipmap<CF>& mipmap, AF addressing) {
		return AnisoSample(u, v, ddx_, ddy_, mipmap, addressing);
	}

	enum SampleCat {
		kNearest = 0,
		kBilinear = 1,
		kTrilinear = 2,
		kAniso = 3
	};

	enum AddresingCat {
		kClamp = 0,
		kRepeat = 1
	};

	template<class TEX>
	typename TEX::format_t::scalar_t sample(float u, float v, const TEX& tex, int sample_cat, int addr_cat) {

		typedef std::function<void(float& u, float& v)> AF;
		AF addressing;
		switch (addr_cat)
		{
		case kClamp:
			addressing = ClampAddr;
			break;
		case kRepeat:
			addressing = RepeatAddr;
			break;
		default:
			break;
		}

		TEX::format_t::scalar_t r;
		switch (sample_cat)
		{
		case kNearest:
			r = this->mipmapNearest(u, v, tex, addressing);
			break;
		case kBilinear:
			r = this->mipmapBilinear(u, v, tex, addressing);
			break;
		case kTrilinear:
			r = this->mipmapTrilinear(u, v, tex, addressing);
			break;
		case kAniso:
			r = this->mipmapAniso(u, v, tex, addressing);
			break;
		default:
			break;
		}

		return r;
	}

public:
	Vector2f ddx_, ddy_;//uvµÄµ¼Êý
};


SHAKURAS_END;