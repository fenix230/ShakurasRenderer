#pragma once
#include "GlRendererDll.h"
#include "Core/Utility.h"
#include <vector>

#pragma warning(push) 
#pragma warning(disable:4251)


SHAKURAS_BEGIN;


class GLRENDERER_DLL GlBatch {
public:
	virtual ~GlBatch() {}
	virtual void draw() = 0;
};


SHAKURAS_SHARED_PTR(GlBatch);


//��װһ��Vertex Array Object
//����ʱ�ͷ��ܿص�gl����
class GLRENDERER_DLL GlVAO : public GlBatch {
public:
	GlVAO();
	virtual ~GlVAO();

public:
	enum Catagory {
		kNil = 0,
		kPoints,
		kLines,
		kLineLoop,
		kLineStrip,
		kTriangles,
		kTriangleStrip,
		kTriangleFan,
		kQuads,
		kQuadStrip,
		kPolygon
	};

	//begin(...)
	//setIndexBuffer(...)
	//setAttribBuffer(...)
	//end()
	//...
	//draw()

	void begin(short cat, uint16_t vert_count, int attrib_count);

	void setIndexBuffer(const uint16_t* buffer, int len);

	void setAttribBuffer(const float* buffer, int index, int size);

	void end();

	virtual void draw();

private:
	unsigned int vao_;
	unsigned int primtype_;
	uint16_t vertcount_;
	unsigned int index_buffer_;
	std::vector<unsigned int> attrib_buffers_;
};


SHAKURAS_SHARED_PTR(GlVAO);


class GLRENDERER_DLL GlVAOFactory {
public:
	GlVAOFactory();

public:
	void reset();

	//ͼԪ���
	void setPrimtiveCat(short cat);

	//������
	void registerAttrib(int index, int size_by_float);

	//��������
	void addIndex(uint16_t vi);

	//��Ӷ���
	uint16_t addVertex();

	void setAttrib1f(int index, float val);
	void setAttrib2f(int index, float val1, float val2);
	void setAttrib2fv(int index, const float* val);
	void setAttrib3f(int index, float val1, float val2, float val3);
	void setAttrib3fv(int index, const float* val);
	void setAttrib4f(int index, float val1, float val2, float val3, float val4);
	void setAttrib4fv(int index, const float* val);

	//������
	uint16_t attribCount(int index) const;

	//���������������δ���룬����0
	uint16_t vertCount() const;

	//����VAO
	GlVAOPtr createVAO();

private:
	short cat_;
	uint16_t vert_count_;
	std::vector<uint16_t> index_;
	std::vector<int> sizebyfloats_;
	std::vector<std::vector<float> > attribs_;
};


SHAKURAS_END;


#pragma warning(pop) 