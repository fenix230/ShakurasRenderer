#pragma once
#include "Utility.h"


SHAKURAS_BEGIN;


enum GsTraversalBehavior {
	kTBLerp = 1UL << 0,//���Բ�ֵ
	kTBPerspect = 1UL << 1,//͸��
	kTBAll = kTBLerp | kTBPerspect
};


SHAKURAS_END;