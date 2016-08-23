#pragma once
#include "SoftClipper.h"
#include "Core/Profiler.h"
#include <ppl.h>


SHAKURAS_BEGIN;


template<class UL, class A, class V, class VS>
class SoftGeometryStage {
public:
	void initialize(float w, float h, Profiler& profiler) {
		width_ = w;
		height_ = h;
		profiler_ = &profiler;
		refuse_back_ = true;
	}

	void process(SoftDrawCall<UL, A, V>& call) {
		profiler_->count("Geo-Triangle Count", (int)call.prims.indexs_.size() / 3);

		//vertex sharding
		//geometry sharding£¨Œ¥ µœ÷
		//projection transform
		auto vert_geom_sharding_and_proj = [&](SoftVertex<A, V>& vert) {
			VS().process(call.uniforms, vert);
		};

		profiler_->count("Vert-Sharder Excuted", (int)call.prims.verts_.size());
		Concurrency::parallel_for_each(call.prims.verts_.begin(), call.prims.verts_.end(), vert_geom_sharding_and_proj);

		//cliping
		clipper_.reset(call.prims, *profiler_, refuse_back_);
		clipper_.process();

		//screen mapping
		auto screen_mapping = [&](SoftVertex<A, V>& vert) {
			screenMapping(vert.pos);
		};
		Concurrency::parallel_for_each(call.prims.verts_.begin(), call.prims.verts_.end(), screen_mapping);
	}

	void refuseBack(bool rb) {
		refuse_back_ = rb;
	}

private:
	void screenMapping(Vector4f& v) {
		float w = v.w;
		float rhw = 1.0f / w;

		Vector4f r;
		r.x = (v.x * rhw + 1.0f) * width_ * 0.5f;
		r.y = (1.0f - v.y * rhw) * height_ * 0.5f;
		r.z = v.z * rhw;
		r.w = w;

		v = r;
	}


private:
	float width_, height_;
	bool refuse_back_;
	Profiler* profiler_;
	SoftClipper<A, V> clipper_;
};


SHAKURAS_END;