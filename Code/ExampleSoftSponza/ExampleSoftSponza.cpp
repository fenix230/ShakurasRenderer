// Example_Soft_Sponza.cpp : 定义控制台应用程序的入口点。
//

#include "SoftRenderer\SoftPhongShading.h"
#include "Core\Application.h"
#include "ResourceParser\TextureLoader.h"
#include "PlatformSpec\WinViewer.h"
#include "ResourceParser\ObjParser.h"


using namespace shakuras;


namespace soft_sponza {

	struct UniformList {
		SoftMipmapPtr texture;
		Vector3f ambient;
		Vector3f diffuse;
		Vector3f specular;
		Matrix44f mvp_trsf;
		Vector3f light_pos;
		Vector3f eye_pos;
	};

	class VertexShader {
	public:
		void process(const UniformList& u, SoftPhongVertex& v) {
			v.varyings.normal = v.attribs.normal;

			v.varyings.eye_dir = u.eye_pos - v.pos.xyz();

			v.varyings.light_dir = u.light_pos - v.pos.xyz();

			v.pos = u.mvp_trsf.transform(v.pos);

			v.varyings.uv = v.attribs.uv;
		}
	};


	class FragmentShader {
	public:
		void process(const UniformList& u, SoftSampler& sampler, SoftPhongFragment& f) {
			Vector3f norm = f.varyings.normal;
			Vector3f eye_dir = f.varyings.eye_dir;
			Vector3f light_dir = f.varyings.light_dir;
			Normalize3(norm);
			Normalize3(eye_dir);
			Normalize3(light_dir);

			Vector2f uv = f.varyings.uv;
			Vector3f diff_color(1.0f, 1.0f, 1.0f);//默认白色
			if (u.texture) {
				diff_color = sampler.mipmapTrilinear(uv.x, uv.y, *u.texture, RepeatAddr);
			}

			float illum_diffuse = Clamp(DotProduct3(light_dir, norm), 0.0f, 1.0f);
			Vector3f c = diff_color * illum_diffuse;

			Clamp(c.x, 0.0f, 1.0f);
			Clamp(c.y, 0.0f, 1.0f);
			Clamp(c.z, 0.0f, 1.0f);

			f.c.set(c.x, c.y, c.z, 1.0f);
		}
	};

	typedef shakuras::SoftDrawCall<UniformList, SoftPhongAttribList, SoftPhongVaryingList> DrawCall;

	class AppStage {
	public:
		bool initialize(WinMemViewerPtr viewer) {
			float w = (float)viewer->width();
			float h = (float)viewer->height();

			std::vector<ObjMesh> meshs;
			LoadObjMesh("Sponza/sponza.obj", meshs, false);

			outputs_.clear();
			outputs_.resize(meshs.size());

			for (size_t i = 0; i != meshs.size(); i++) {
				const ObjMesh& mesh = meshs[i];
				DrawCall& cmd = outputs_[i];

				cmd.prims.verts_.resize(mesh.verts.size());
				for (size_t ii = 0; ii != mesh.verts.size(); ii++) {
					const ObjVert& objv = mesh.verts[ii];
					SoftPhongVertex& v = cmd.prims.verts_[ii];

					v.pos.set(objv.pos.x, objv.pos.y, objv.pos.z, 1.0f);
					v.attribs.uv = objv.uv;
					v.attribs.normal = objv.normal;
				}

				cmd.prims.indexs_ = mesh.tris;

				proj_ = Matrix44f::Perspective(kGSPI * 0.5f, w / h, 5.0f, 1000.0f);//投影变换

				SoftSurfacePtr surface = std::make_shared<SoftSurface>();
				int texw = 0, texh = 0;
				void* bits = LoadTexture(mesh.mtl.tex_full_path, false, texw, texh);
				surface->reset(texw, texh, (uint32_t*)bits);
				ResFree(bits);
				cmd.uniforms.texture = CreateSoftMipmap(surface);

				cmd.uniforms.ambient = mesh.mtl.ambient;
				cmd.uniforms.diffuse = mesh.mtl.diffuse;
				cmd.uniforms.specular = mesh.mtl.specular;
			}

			step_ = 0;
			move_ = 0;

			viewer_ = viewer;

			return true;
		}

		void process(std::vector<DrawCall>& cmds) {
			if (viewer_->testUserMessage(kUMUp)) step_++;
			if (viewer_->testUserMessage(kUMDown)) step_--;
			if (viewer_->testUserMessage(kUMLeft)) step_++;
			if (viewer_->testUserMessage(kUMRight)) step_--;

			float xpos = -30.0f + fmodf(0.625f * step_, 66.0f);
			float ypos = 10.0f + fmodf(0.5f * step_, 40.0f);

			Vector3f eye(xpos, 12.5f, 0.0f), at(40.0f, 15.0f, 0.0f), up(0.0f, 1.0f, 0.0f);
			Vector3f eye_pos = eye;
			Vector3f light_pos(0.0f, ypos, 0.0f);

			Matrix44f modeltrsf = Matrix44f::Translate(-0.5f, 0.0f, -0.5f);
			Matrix44f viewtrsf = Matrix44f::LookAt(eye, at, up);

			for (size_t i = 0; i != outputs_.size(); i++) {
				DrawCall& cmd = outputs_[i];

				cmd.uniforms.mvp_trsf = modeltrsf * viewtrsf * proj_;//模型*视图*投影
				cmd.uniforms.eye_pos = eye_pos;//相机位置
				cmd.uniforms.light_pos = light_pos;//光源位置
			}

			cmds = outputs_;
		}

	private:
		WinMemViewerPtr viewer_;
		Matrix44f proj_;
		std::vector<DrawCall> outputs_;
		int step_, move_;
	};

	typedef shakuras::SoftRenderStage<UniformList, SoftPhongAttribList, SoftPhongVaryingList, Vector4f, VertexShader, FragmentShader> RenderStage;

	typedef shakuras::Application<DrawCall, AppStage, RenderStage> Application;
}


int main()
{
	const char *title = "ShakurasRenderer - "
		"Left/Right: rotation, Up/Down: forward/backward";

	int width = 512, height = 512;
	WinMemViewerPtr viewer = std::make_shared<WinMemViewer>();
	if (!viewer || viewer->initialize(width, height, title) != 0) {
		return -1;
	}

	soft_sponza::Application app;
	app.initialize(viewer);
	app.renstage_.geostage_.refuseBack(false);

	while (!viewer->testUserMessage(kUMEsc) && !viewer->testUserMessage(kUMClose)) {
		viewer->dispatch();

		app.process();

		viewer->update();
		Sleep(1);
	}

	return 0;
}


