#pragma once
#include "SoftGeometryStage.h"
#include "SoftVertex.h"
#include "SoftSampler.h"
#include "SoftColorFormat.h"
#include "Core/Profiler.h"
#include <vector>
#include <array>
#include <ppl.h>


SHAKURAS_BEGIN;


struct Edge {
	Vector3f v1, v2;
};


inline Vector3f Lerp(const Edge& edge, float y) {
	return edge.v1 + (edge.v2 - edge.v1) * ((y - edge.v1.y) / (edge.v2.y - edge.v1.y));
}


struct Trapezoid {
	float top, bottom;
	Edge left, right;
};


template<class A, class V>
inline Vector3f XYRhw(const SoftVertex<A, V>& v) {
	return Vector3f(v.pos.x, v.pos.y, v.rhw);
}


template<class A, class V>
int SpliteTrapezoid(const SoftVertex<A, V>& v0, const SoftVertex<A, V>& v1, const SoftVertex<A, V>& v2, std::vector<Trapezoid>& traps) {
	const SoftVertex<A, V>* p1 = &v0;
	const SoftVertex<A, V>* p2 = &v1;
	const SoftVertex<A, V>* p3 = &v2;
	float k, x;

	if (p1->pos.y > p2->pos.y) std::swap(p1, p2);
	if (p1->pos.y > p3->pos.y) std::swap(p1, p3);
	if (p2->pos.y > p3->pos.y) std::swap(p2, p3);
	if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;
	if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;

	traps.push_back(Trapezoid());
	if (p1->pos.y == p2->pos.y) {	// triangle down
		if (p1->pos.x > p2->pos.x) std::swap(p1, p2);
		traps[0].top = p1->pos.y;
		traps[0].bottom = p3->pos.y;
		traps[0].left.v1 = XYRhw(*p1);
		traps[0].left.v2 = XYRhw(*p3);
		traps[0].right.v1 = XYRhw(*p2);
		traps[0].right.v2 = XYRhw(*p3);
		if (traps[0].top < traps[0].bottom) {
			return 1;
		}
		else {
			traps.clear();
			return 0;
		}
	}

	if (p2->pos.y == p3->pos.y) {	// triangle up
		if (p2->pos.x > p3->pos.x) std::swap(p2, p3);
		traps[0].top = p1->pos.y;
		traps[0].bottom = p3->pos.y;
		traps[0].left.v1 = XYRhw(*p1);
		traps[0].left.v2 = XYRhw(*p2);
		traps[0].right.v1 = XYRhw(*p1);
		traps[0].right.v2 = XYRhw(*p3);
		if (traps[0].top < traps[0].bottom) {
			return 1;
		}
		else {
			traps.clear();
			return 0;
		}
	}

	traps.push_back(Trapezoid());
	traps[0].top = p1->pos.y;
	traps[0].bottom = p2->pos.y;
	traps[1].top = p2->pos.y;
	traps[1].bottom = p3->pos.y;

	k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
	x = p1->pos.x + (p2->pos.x - p1->pos.x) * k;

	if (x <= p3->pos.x) {		// triangle left
		traps[0].left.v1 = XYRhw(*p1);
		traps[0].left.v2 = XYRhw(*p2);
		traps[0].right.v1 = XYRhw(*p1);
		traps[0].right.v2 = XYRhw(*p3);
		traps[1].left.v1 = XYRhw(*p2);
		traps[1].left.v2 = XYRhw(*p3);
		traps[1].right.v1 = XYRhw(*p1);
		traps[1].right.v2 = XYRhw(*p3);
	}
	else {					// triangle right
		traps[0].left.v1 = XYRhw(*p1);
		traps[0].left.v2 = XYRhw(*p3);
		traps[0].right.v1 = XYRhw(*p1);
		traps[0].right.v2 = XYRhw(*p2);
		traps[1].left.v1 = XYRhw(*p1);
		traps[1].left.v2 = XYRhw(*p3);
		traps[1].right.v1 = XYRhw(*p2);
		traps[1].right.v2 = XYRhw(*p3);
	}

	return 2;
}


//每个像素一个片元，每个片元一个采样点
template<class FRAG>
class TrapTraversal {
public:
	TrapTraversal(const Trapezoid& trap, int width, int height, std::vector<std::array<FRAG, 4> >& output) {
		trap_ = &trap;
		width_ = width;
		height_ = height;
		output_ = &output;
	}

public:
	void process() {
		for (int i = (int)trap_->top; i <= (int)(trap_->bottom); i += 2) {
			scan(i);
		}
	}

private:
	struct RangeLine {
		Vector3f left, right;
		int xb, xe;
		bool visible;
	};

	void scan(int y0) {
		int y1 = y0 + 1;
		float yf0 = y0 + 0.5f;
		float yf1 = y0 + 1.5f;

		RangeLine rl0, rl1;

		rl0.visible = (trap_->top <= yf0 && yf0 <= trap_->bottom && 0 <= y0 && y0 < height_);
		rl1.visible = (trap_->top <= yf1 && yf1 <= trap_->bottom && 0 <= y1 && y1 < height_);

		rl0.left = Lerp(trap_->left, yf0);
		rl0.right = Lerp(trap_->right, yf0);
		rl1.left = Lerp(trap_->left, yf1);
		rl1.right = Lerp(trap_->right, yf1);

		rl0.xb = (std::max)(0, (int)(rl0.left.x + 0.5f));
		rl0.xe = (std::min)(width_, (int)(rl0.right.x + 0.5f));
		rl1.xb = (std::max)(0, (int)(rl1.left.x + 0.5f));
		rl1.xe = (std::min)(width_, (int)(rl1.right.x + 0.5f));

		int xb = 0, xe = 0;
		if (rl0.visible && rl1.visible) {
			xb = (std::min)(rl0.xb, rl1.xb);
			xe = (std::max)(rl0.xe, rl1.xe);
		}
		else if (rl0.visible) {
			xb = rl0.xb;
			xe = rl0.xe;
		}
		else if (rl1.visible) {
			xb = rl1.xb;
			xe = rl1.xe;
		}
		else {
			return;
		}

		for (int i = xb; i < xe; i += 2) {
			float xf0 = i + 0.5f;
			float xf1 = i + 1.5f;

			//2, 3
			//0, 1
			std::array<FRAG, 4> tile;
			fragAssign(xf0, yf0, rl0, tile[0]);
			fragAssign(xf1, yf0, rl0, tile[1]);
			fragAssign(xf0, yf1, rl1, tile[2]);
			fragAssign(xf1, yf1, rl1, tile[3]);

			output_->push_back(tile);
		}
	}

	void fragAssign(float xf, float yf, const RangeLine& rl, FRAG& frag) {
		frag.x = (int)xf;
		frag.y = (int)yf;
		frag.z = Lerp(rl.left.z, rl.right.z, (xf - rl.left.x) / (rl.right.x - rl.left.x));
		frag.weight = (rl.visible && (rl.xb <= frag.x && frag.x < rl.xe) ? 1.0f : 0.0f);
	}

public:
	const Trapezoid* trap_;
	int width_, height_;
	std::vector<std::array<FRAG, 4> >* output_;
};


template<class UL, class FRAG, class FS>
class TileShader {
public:
	void process(const UL& u, std::array<FRAG, 4>& tile) {
		sampler_.ddx_ = TexCoord(tile[1].varyings) - TexCoord(tile[0].varyings);
		sampler_.ddy_ = TexCoord(tile[2].varyings) - TexCoord(tile[0].varyings);
		for (int i = 0; i != 4; i++) {
			if (0.0f < tile[i].weight) {
				fragshader_.process(u, sampler_, tile[i]);
			}
		}
	}

public:
	SoftSampler sampler_;
	FS fragshader_;
};


template<class VERT, class FRAG>
class LerpDerivative {
public:
	void setTriangle(const VERT& v0, const VERT& v1, const VERT& v2) {
		VERT e01 = v1 - v0;
		VERT e02 = v2 - v0;

		float area = e02.pos.x * e01.pos.y - e01.pos.x * e02.pos.y;
		if (area == 0.0f) area = 0.000001f;
		float inv_area = 1.0f / area;

		v0_ = v0;
		ddx_ = (e02 * e01.pos.y - e01 * e02.pos.y) * inv_area;
		ddy_ = (e01 * e02.pos.x - e02 * e01.pos.x) * inv_area;
	}

	void lerp(FRAG& frag) const {
		float fx = frag.x + 0.5f;
		float fy = frag.y + 0.5f;
		float rhw = frag.z;
		if (rhw == 0.0f) rhw = 0.000001f;
		frag.varyings = (v0_.varyings + ddx_.varyings * (fx - v0_.pos.x) + ddy_.varyings * (fy - v0_.pos.y)) / rhw;
		frag.z = rhw;
	}

private:
	VERT v0_, ddy_, ddx_;
};


template<class UL, class A, class V, class CF, class FS>
class SoftRasterizerStage {
public:
	typedef typename CF::data_t color_data_t;
	typedef typename CF::scalar_t color_scalar_t;
	typedef SoftVertex<A, V> vertex_t;
	typedef SoftFragment<V, color_scalar_t> fragment_t;


public:
	void initialize(int ww, int hh, void* fb, Profiler& profiler) {
		width_ = ww;
		height_ = hh;
		profiler_ = &profiler;

		framebuffer_.resize(height_, nullptr);
		depthbuffer_.resize(height_);

		for (int y = 0; y != height_; y++) {
			depthbuffer_[y].resize(width_, 0.0f);
		}

		color_data_t* framebuf = (color_data_t*)fb;
		for (int i = 0; i < height_; i++) {
			framebuffer_[i] = framebuf + width_ * i;
		}
	}

	void process(SoftDrawCall<UL, A, V>& call) {
		profiler_->count("Ras-Triangle Count", (int)call.prims.indexs_.size() / 3);
		
		//triangle setup, 省略

		for (size_t i = 0; i + 2 < call.prims.indexs_.size(); i += 3) {
			//triangle traversal
			//fragment shader
			//merging
			const size_t* tri = &call.prims.indexs_[i];

			const vertex_t& v1 = call.prims.verts_[tri[0]];
			const vertex_t& v2 = call.prims.verts_[tri[1]];
			const vertex_t& v3 = call.prims.verts_[tri[2]];
			drawTriangle(call.uniforms, v1, v2, v3);
		}
	}

	void clean() {
		for (int y = 0; y < height_; y++) {
			color_data_t *dst = framebuffer_[y];
			for (int x = width_; x > 0; dst++, x--) dst[0] = CF::data(CF::clean());
		}

		for (int y = 0; y < height_; y++) {
			std::vector<float>& dst = depthbuffer_[y];
			std::fill(dst.begin(), dst.end(), 0.0f);
		}
	}

private:
	void drawTriangle(const UL& u, vertex_t v0, vertex_t v1, vertex_t v2) {
		v0.rhwInitialize();
		v1.rhwInitialize();
		v2.rhwInitialize();

		LerpDerivative<vertex_t, fragment_t> lerpd;
		lerpd.setTriangle(v0, v1, v2);

		std::vector<Trapezoid> traps;
		SpliteTrapezoid(v0, v1, v2, traps);

		for (auto i = traps.begin(); i != traps.end(); i++) {
			drawTrapezoid(u, lerpd, *i);
		}
	}

	void drawTrapezoid(const UL& u, const LerpDerivative<vertex_t, fragment_t>& lerpd, Trapezoid& trap) {
		std::vector<std::array<fragment_t, 4> > tiles;

		TrapTraversal<fragment_t>(trap, width_, height_, tiles).process();

		profiler_->count("Frag Count", (int)(4 * tiles.size()));
		for (auto i = tiles.begin(); i != tiles.end(); i++) {
			int incr = 0;
			incr += (0.0f < (*i)[0].weight ? 1 : 0);
			incr += (0.0f < (*i)[1].weight ? 1 : 0);
			incr += (0.0f < (*i)[2].weight ? 1 : 0);
			incr += (0.0f < (*i)[3].weight ? 1 : 0);
			profiler_->count("Frag-Sharder Excuted", incr);
		}

		//fragment lerp
		//fragment sharding
		auto frag_lerp_and_sharding = [&](std::array<fragment_t, 4>& tile) {
			lerpd.lerp(tile[0]);
			lerpd.lerp(tile[1]);
			lerpd.lerp(tile[2]);
			lerpd.lerp(tile[3]);

			TileShader<UL, fragment_t, FS>().process(u, tile);
		};

		Concurrency::parallel_for_each(tiles.begin(), tiles.end(), frag_lerp_and_sharding);

		//merging
		for (auto i = tiles.begin(); i != tiles.end(); i++) {
			merge(*i);
		}
	}

	void merge(const std::array<fragment_t, 4>& tile) {
		for (size_t i = 0; i != 4; i++) {
			const fragment_t& frag = tile[i];
			if (frag.weight != 0.0f && depthbuffer_[frag.y][frag.x] <= frag.z) {
				depthbuffer_[frag.y][frag.x] = frag.z;
				framebuffer_[frag.y][frag.x] = CF::data(frag.c);
			}
		}
	}

private:
	std::vector<color_data_t*> framebuffer_;
	std::vector<std::vector<float> > depthbuffer_;
	int width_, height_;
	Profiler* profiler_;
};


SHAKURAS_END;