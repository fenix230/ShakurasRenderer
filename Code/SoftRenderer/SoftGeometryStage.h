#pragma once
#include "SoftClipper.h"
#include "Core/Profiler.h"
#include <ppl.h>


SHAKURAS_BEGIN;


template<class UL, class V, class VS>
class SoftGeometryStage {
public:
	void initialize(float w, float h, Profiler& profiler) {
		width_ = w;
		height_ = h;
		profiler_ = &profiler;
		refuse_back_ = true;
	}

	void process(SoftDrawCall<UL, V>& call) {
		profiler_->count("Geo-Triangle Count", (int)call.prims.tris_.size());

		//vertex sharding
		//geometry sharding��δʵ��
		//projection transform
		auto vert_geom_sharding_and_proj = [&](V& vert) {
			VS().process(call.uniforms, vert);
			vert.pos = call.proj_trsf.transform(vert.pos);
		};

		profiler_->count("Vert-Sharder Excuted", (int)call.prims.verts_.size());
		Concurrency::parallel_for_each(call.prims.verts_.begin(), call.prims.verts_.end(), vert_geom_sharding_and_proj);

		//cliping
		clipper_.reset(call.prims, *profiler_, refuse_back_);
		clipper_.process();

		//screen mapping
		auto screen_mapping = [&](V& vert) {
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
	SoftClipper<V> clipper_;
};


SHAKURAS_END;