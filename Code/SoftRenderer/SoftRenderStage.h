#pragma once
#include "SoftDrawCall.h"
#include "SoftRasterizerStage.h"
#include "SoftGeometryStage.h"
#include "Core/Profiler.h"


SHAKURAS_BEGIN;


template<class UL, class A, class V, class C, class VS, class FS>
class SoftRenderStage {
public:
	template<class VPTR>
	void initialize(VPTR viewer, Profiler& profiler) {
		geostage_.initialize((float)viewer->width(), (float)viewer->height(), profiler);
		rasstage_.initialize(viewer->width(), viewer->height(), viewer->frameBuffer(), profiler);
	}

	void process(std::vector<SoftDrawCall<UL, A, V> >& calls) {
		rasstage_.clean();

		for (auto i = calls.begin(); i != calls.end(); i++) {
			geostage_.process(*i);
			rasstage_.process(*i);
		}
	}

public:
	SoftGeometryStage<UL, A, V, VS> geostage_;
	SoftRasterizerStage<UL, A, V, C, FS> rasstage_;
};


SHAKURAS_END;