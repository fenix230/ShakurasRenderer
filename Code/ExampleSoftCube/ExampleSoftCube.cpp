// Example_Soft_Cube.cpp : 定义控制台应用程序的入口点。
//


#include <vector>
#include "SoftRenderer\SoftPhongShading.h"
#include "Core\Application.h"
#include "ResourceParser\TextureLoader.h"
#include "PlatformSpec\WinViewer.h"


using namespace shakuras;


namespace soft_cube {

	SoftMipmapU32F3Ptr LoadMipmap(std::string tex_full_path) {
		SoftSurfaceU32F3Ptr surface = std::make_shared<SoftSurfaceU32F3>();
		int texw = 0, texh = 0;
		void* bits = LoadTexture(tex_full_path, true, texw, texh);
		surface->reset(texw, texh, (uint32_t*)bits, Swap02);
		ResFree(bits);
		return CreateSoftMipmap(surface);
	}

	SoftMipmapU32F3Ptr GridMipmap() {
		SoftSurfaceU32F3Ptr surface = std::make_shared<SoftSurfaceU32F3>();
		int texw = 0, texh = 0;
		void* bits = GridTexture(texw, texh);
		surface->reset(texw, texh, (uint32_t*)bits, Assign);
		return CreateSoftMipmap(surface);
	}

	void GenerateCube(SoftPhongPrimitiveList& prims) {
		static SoftPhongVertex mesh[8] = {
			{ { -1, -1, -1, 1 } },
			{ { 1, -1, -1, 1 } },
			{ { 1, 1, -1, 1 } },
			{ { -1, 1, -1, 1 } },
			{ { -1, -1, 1, 1 } },
			{ { 1, -1, 1, 1 } },
			{ { 1, 1, 1, 1 } },
			{ { -1, 1, 1, 1 } },
		};

		auto draw_plane = [&](int a, int b, int c, int d) {
			SoftPhongVertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];

			p1.attribs.uv.set(0, 0);
			p2.attribs.uv.set(0, 1);
			p3.attribs.uv.set(1, 1);
			p4.attribs.uv.set(1, 0);

			Vector3f norm = CrossProduct3((p3.pos - p2.pos).xyz(), (p1.pos - p2.pos).xyz());
			p1.attribs.normal = norm;
			p2.attribs.normal = norm;
			p3.attribs.normal = norm;
			p4.attribs.normal = norm;

			size_t index = prims.verts_.size();
			prims.verts_.push_back(p1);
			prims.verts_.push_back(p2);
			prims.verts_.push_back(p3);
			prims.indexs_.push_back(index);
			prims.indexs_.push_back(index + 1);
			prims.indexs_.push_back(index + 2);

			index = prims.verts_.size();
			prims.verts_.push_back(p3);
			prims.verts_.push_back(p4);
			prims.verts_.push_back(p1);
			prims.indexs_.push_back(index);
			prims.indexs_.push_back(index + 1);
			prims.indexs_.push_back(index + 2);
		};

		draw_plane(0, 3, 2, 1);
		draw_plane(4, 5, 6, 7);
		draw_plane(0, 1, 5, 4);
		draw_plane(1, 2, 6, 5);
		draw_plane(2, 3, 7, 6);
		draw_plane(0, 4, 7, 3);
	}

	class AppStage {
	public:
		bool initialize(WinMemViewerPtr viewer) {
			float w = (float)viewer->width();
			float h = (float)viewer->height();

			texlist_.push_back(LoadMipmap("Cube/1.png"));
			texlist_.push_back(LoadMipmap("Cube/1.jpg"));
			texlist_.push_back(LoadMipmap("Cube/2.png"));
			texlist_.push_back(GridMipmap());
			itex_ = 0;
			nspace_ = 0;

			GenerateCube(output_.prims);
			proj_ = Matrix44f::Perspective(kGSPI * 0.6f, w / h, 1.0f, 500.0f);//投影变换
			output_.uniforms.texture = texlist_[itex_];//纹理
			output_.uniforms.ambient.set(0.4f, 0.4f, 0.4f);//环境光
			output_.uniforms.diffuse.set(0.587609f, 0.587609f, 0.587609f);//漫反射
			output_.uniforms.specular.set(0.071744f, 0.071744f, 0.071744f);//镜面反射

			alpha_ = 0.0f;
			pos_ = 3.5f;

			viewer_ = viewer;

			return true;
		}

		void process(std::vector<SoftPhongDrawCall>& cmds) {
			if (viewer_->testUserMessage(kUMSpace)) {
				if (++nspace_ == 1) {
					itex_ = (itex_ + 1) % texlist_.size();
				}
			}
			else {
				nspace_ = 0;
			}
			if (viewer_->testUserMessage(kUMUp)) pos_ += 0.04f;
			if (viewer_->testUserMessage(kUMDown)) pos_ -= 0.04f;
			if (viewer_->testUserMessage(kUMLeft)) alpha_ -= 0.02f;
			if (viewer_->testUserMessage(kUMRight)) alpha_ += 0.02f;

			Vector3f eye(0, -3 - pos_, 2.0f), at(0, 0, 0), up(0, 0, 1);
			Vector3f eye_pos = eye;
			Vector3f light_dir(-1.0f, -1.0f, 1.0f);

			Matrix44f modeltrsf = Matrix44f::Rotate(0.0f, 0.0f, 1.0f, alpha_);
			Matrix44f viewtrsf = Matrix44f::LookAt(eye, at, up);

			output_.uniforms.texture = texlist_[itex_];//纹理
			output_.uniforms.model_trsf = modeltrsf;//模型变换
			output_.uniforms.mvp_trsf = modeltrsf * viewtrsf * proj_;//模型*视图变换*投影变换
			output_.uniforms.eye_pos = eye_pos;//相机位置
			output_.uniforms.light_dir = light_dir;//光源位置

			cmds.push_back(output_);
		}

	private:
		WinMemViewerPtr viewer_;
		SoftPhongDrawCall output_;
		Matrix44f proj_;
		std::vector<SoftMipmapU32F3Ptr> texlist_;
		int itex_;
		int nspace_;
		float alpha_;
		float pos_;
	};

	typedef shakuras::Application<SoftPhongDrawCall, AppStage, SoftPhongRenderStage> Application;
}


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward, Space: switch texture";

	int width = 1024, height = 768;
	WinMemViewerPtr viewer = std::make_shared<WinMemViewer>();
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	soft_cube::Application app;
	app.initialize(viewer);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		app.process();

		viewer->update();
		Sleep(1);
	}

	return 0;
}
