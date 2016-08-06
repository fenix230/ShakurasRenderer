#pragma once
#include "Core/Profiler.h"


SHAKURAS_BEGIN;


template<class CALL, class GS, class RS>
class SoftRenderStage {
public:
	template<class VPTR>
	void initialize(VPTR viewer, Profiler& profiler) {
		geostage_.initialize((float)viewer->width(), (float)viewer->height(), profiler);
		rasstage_.initialize(viewer->width(), viewer->height(), viewer->frameBuffer(), profiler);
	}

	void process(std::vector<CALL>& calls) {
		rasstage_.clean();

		for (auto i = calls.begin(); i != calls.end(); i++) {
			geostage_.process(*i);
			rasstage_.process(*i);
		}
	}

public:
	GS geostage_;
	RS rasstage_;
};


SHAKURAS_END;