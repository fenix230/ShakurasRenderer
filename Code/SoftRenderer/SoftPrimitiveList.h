#pragma once
#include "SoftVertex.h"
#include <assert.h>


SHAKURAS_BEGIN;


template<class A, class V> 
class SoftPrimitiveList {
public:
	void clear() {
		verts_.clear();
		indexs_.clear();
	}

public:
	std::vector<SoftVertex<A, V> > verts_;
	std::vector<size_t> indexs_;//Ò»¶¨ÊÇTriangles
};


SHAKURAS_END;