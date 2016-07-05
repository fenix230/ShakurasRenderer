#pragma once
#include "Utility.h"
#include "GsViewer.h"
#include "GsVertexFragment.h"
#include "GsTexture.h"
#include <vector>
#include <array>


SHAKURAS_BEGIN;


class GsApplicationStage {
public:
	struct Out {
		float width;
		float height;

		Matrix44f viewtrsf;
		Matrix44f projtrsf;

		std::vector<GsVertex> vertlist;
		std::vector<Matrix44f> modeltrsflist;
		std::vector<GsTextureU32Ptr> texturelist;

		std::vector<std::array<int, 3> > itris;//vertlist ����
		std::vector<int> itexs;//texturelist ����
		std::vector<int> itrsfs;//modeltrsflist ����
	};


	virtual void initialize(GsViewerPtr viewer);
	virtual void process(Out& output);

private:
	GsViewerPtr viewer_;
	Out output_;
	float alpha_;
	float pos_;
};


SHAKURAS_END;