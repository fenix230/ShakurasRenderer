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

	void begin(short cat, uint16_t vert_count, int attrib_count, bool is_static);

	void setIndexBuffer(const uint16_t* buffer, int len);

	void setAttribBuffer(const float* buffer, int index, int size);

	void end();

	virtual void draw();

private:
	unsigned int vao_;
	unsigned int primtype_;
	bool isstatic_;
	uint16_t vertcount_;
	unsigned int index_buffer_;
	std::vector<int> attib_sizebyfloats_;
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
	void setAttribCount(int attrib_count);

	//��̬ͼԪ
	void setStatic(bool is_static);

	//��������
	void addIndex(uint16_t vi);

	//��������
	void addVertexAttrib1f(int index, float val);
	void addVertexAttrib2f(int index, float val1, float val2);
	void addVertexAttrib2fv(int index, const float* val);
	void addVertexAttrib3f(int index, float val1, float val2, float val3);
	void addVertexAttrib3fv(int index, const float* val);
	void addVertexAttrib4f(int index, float val1, float val2, float val3, float val4);
	void addVertexAttrib4fv(int index, const float* val);

	//������
	uint16_t attribCount(int index) const;

	//���������������δ���룬����0
	uint16_t vertCount() const;

	//����VAO
	GlVAOPtr createVAO();

private:
	short cat_;
	int attrib_count_;
	uint16_t vertex_count_;
	bool is_static_;
	std::vector<uint16_t> index_;
	std::vector<int> attib_sizebyfloats_;
	std::vector<std::vector<float> > attribs_;
};


SHAKURAS_END;


#pragma warning(pop) 