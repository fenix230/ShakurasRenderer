#pragma once
#include "SoftMipmap.h"
#include "SoftVertex.h"
#include "SoftFragment.h"
#include "SoftDrawCall.h"
#include "SoftGeometryStage.h"
#include "SoftRasterizerStage.h"
#include "SoftRenderStage.h"
#include "Core/MathAndGeometry.h"


SHAKURAS_BEGIN;


//典型 Fixed-Function Pipeline 的 Phong 着色
struct SoftPhongUniformList {
	SoftMipmapPtr texture;
	Vector3f ambient;
	Vector3f diffuse;
	Vector3f specular;
	Matrix44f model_trsf;
	Matrix44f mvp_trsf;
	Vector3f light_dir;
	Vector3f eye_pos;
};


struct SoftPhongAttribList {
	Vector2f uv;
	Vector3f normal;
};


struct SoftPhongVaryingList {
	Vector2f uv;
	Vector3f normal;
	Vector3f light_dir;
	Vector3f eye_dir;
};

template<>
inline Vector2f TexCoord<SoftPhongVaryingList>(const SoftPhongVaryingList& v) {
	return v.uv;
}

inline SoftPhongVaryingList operator+(const SoftPhongVaryingList& v1, const SoftPhongVaryingList& v2) {
	SoftPhongVaryingList v3;
	v3.uv = v1.uv + v2.uv;
	v3.normal = v1.normal + v2.normal;
	v3.light_dir = v1.light_dir + v2.light_dir;
	v3.eye_dir = v1.eye_dir + v2.eye_dir;
	return v3;
}

inline SoftPhongVaryingList operator-(const SoftPhongVaryingList& v1, const SoftPhongVaryingList& v2) {
	SoftPhongVaryingList v3;
	v3.uv = v1.uv - v2.uv;
	v3.normal = v1.normal - v2.normal;
	v3.light_dir = v1.light_dir - v2.light_dir;
	v3.eye_dir = v1.eye_dir - v2.eye_dir;
	return v3;
}

inline SoftPhongVaryingList operator*(const SoftPhongVaryingList& v1, float t) {
	SoftPhongVaryingList v3;
	v3.uv = v1.uv * t;
	v3.normal = v1.normal * t;
	v3.light_dir = v1.light_dir * t;
	v3.eye_dir = v1.eye_dir * t;
	return v3;
}

inline SoftPhongVaryingList operator/(const SoftPhongVaryingList& v1, float d) {
	SoftPhongVaryingList v3;
	float t = 1.0f / d;
	v3.uv = v1.uv * t;
	v3.normal = v1.normal * t;
	v3.light_dir = v1.light_dir * t;
	v3.eye_dir = v1.eye_dir * t;
	return v3;
}


typedef SoftVertex<SoftPhongAttribList, SoftPhongVaryingList> SoftPhongVertex;

typedef SoftFragment<SoftPhongVaryingList, Vector4f> SoftPhongFragment;

typedef SoftDrawCall<SoftPhongUniformList, SoftPhongAttribList, SoftPhongVaryingList> SoftPhongDrawCall;

typedef SoftPrimitiveList<SoftPhongAttribList, SoftPhongVaryingList> SoftPhongPrimitiveList;


class SoftPhongVertexShader {
public:
	void process(const SoftPhongUniformList& u, SoftPhongVertex& v) {
		v.varyings.uv = v.attribs.uv;

		v.varyings.normal = u.model_trsf.transform(v.attribs.normal.xyzw(0.0f)).xyz();

		v.varyings.light_dir = u.light_dir;

		v.varyings.eye_dir = u.eye_pos - v.pos.xyz();

		v.pos = u.mvp_trsf.transform(v.pos);
	}
};


class SoftPhongFragmentShader {
public:
	void process(const SoftPhongUniformList& u, SoftSampler& sampler, SoftPhongFragment& f) {
		Vector3f norm = f.varyings.normal;
		Vector3f light_dir = f.varyings.light_dir;
		Vector3f eye_dir = f.varyings.eye_dir;
		Normalize3(norm);
		Normalize3(light_dir);
		Normalize3(eye_dir);

		float illum_diffuse = Clamp(DotProduct3(light_dir, norm), 0.0f, 1.0f);
		float illum_specular = Clamp(DotProduct3(Reflect(light_dir, norm), eye_dir), 0.0f, 1.0f);
		Vector3f illum = u.ambient + u.diffuse * illum_diffuse + u.specular * illum_specular;

		Vector2f uv = f.varyings.uv;
		Vector3f tc(1.0f, 1.0f, 1.0f);//默认白色
		if (u.texture) {
			tc = sampler.mipmapTrilinear(uv.x, uv.y, *u.texture, RepeatAddr);
		}
		Vector3f c(tc.x * illum.x, tc.y * illum.y, tc.z * illum.z);

		Clamp(c.x, 0.0f, 1.0f);
		Clamp(c.y, 0.0f, 1.0f);
		Clamp(c.z, 0.0f, 1.0f);

		f.c.set(c.x, c.y, c.z, 1.0f);
	}
};


typedef SoftRenderStage<SoftPhongUniformList, SoftPhongAttribList, SoftPhongVaryingList, Vector4f, SoftPhongVertexShader, SoftPhongFragmentShader> SoftPhongRenderStage;


SHAKURAS_END;