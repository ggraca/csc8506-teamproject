#pragma once
#include "../../Common/RendererBase.h"
#include "../../Common/Assets.h"
#include "OGLPixOpsFLags.h"

#include "../../Common/Vector2.h"
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"
#include "../../Common/Matrix4.h"


#ifdef _WIN32
#include "windows.h"
#endif

#include <string>
#include <vector>


namespace NCL {
	class MeshGeometry;
	namespace Rendering {
		class ShaderBase;
		class TextureBase;

		class OGLMesh;
		class OGLShader;

		class SimpleFont;

		class OGLRenderer : public RendererBase
		{
		public:
			friend class OGLRenderer;
			OGLRenderer(Window& w);
			~OGLRenderer();

			void OnWindowResize(int w, int h)	override;

			void DrawString(const std::string& text, const Vec2&pos, const Vec4& colour = Vec4(0.75f, 0.75f, 0.75f, 1), const float& size = 1.0f);

			void DrawLine(const Vec3& start, const Vec3& end, const Vec4& colour);

		protected:			
			void BeginFrame()	override;
			void RenderFrame()	override;
			void EndFrame()		override;

			void DrawDebugData();
			void DrawDebugStrings();
			void DrawDebugLines();
			virtual void SetupDebugMatrix(OGLShader*s) {
			}

			void GenerateIrradianceMap(TextureBase* skybox, TextureBase* irradianceMap, ShaderBase* convShader, MeshGeometry* cube, void* ConvFBO);

			void GenerateFrameBuffer(void* buffer, std::vector<TextureBase*>& bufferTexs, TextureBase* depth);
			void DeleteFrameBuffer(void* buffer);
			void BindFBO(void* buffer);

			void BindShader(ShaderBase*s);
			void BindTextureToShader(const TextureBase*t, const std::string& uniform, int texUnit) const;
			void BindTextureCubeToShader(const TextureBase*t, const std::string& uniform, int texUnit) const;
			void BindBoolToShader(const bool val, const std::string& uniform) const;
			void BindIntToShader(const int val, const std::string& uniform) const;
			void BindFloatToShader(const float val, const std::string& uniform) const;
			void BindVector2ToShader(const Vec2& val, const std::string& uniform) const;
			void BindVector3ToShader(const Vec3& val, const std::string& uniform) const;
			void BindVector4ToShader(const Vec4& val, const std::string& uniform) const;
			void BindMatrix4ToShader(const Mat4& val, const std::string& uniform) const;
			void BindMesh(MeshGeometry*m);
			void DrawBoundMesh(int subLayer = 0, int numInstances = 1);

			void ClearBuffer(bool depth, bool color, bool stencil) const;
			void SetViewport(int x, int y, int width, int height) const;

			OGLPixOpsFLags pixOps;
#ifdef _WIN32
			void InitWithWin32(Window& w);
			void DestroyWithWin32();
			HDC		deviceContext;		//...Device context?
			HGLRC	renderContext;		//Permanent Rendering Context
#endif
		private:
			struct DebugString {
				Maths::Vec4 colour;
				Maths::Vec2	ndcPos;
				float			size;
				std::string		text;
			};

			struct DebugLine {
				Maths::Vec3 start;
				Maths::Vec3 end;
				Maths::Vec4 colour;
			};

			OGLMesh*	boundMesh;
			OGLShader*	boundShader;

			OGLShader*  debugShader;
			SimpleFont* font;
			std::vector<DebugString>	debugStrings;
			std::vector<DebugLine>		debugLines;
		};
	}
}
