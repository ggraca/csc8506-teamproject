#pragma once
#include "../../Common/Matrix4.h"
#include "../../Common/MeshGeometry.h"
class ShaderBase;
class TextureBase;

namespace NCL {
	namespace Rendering {
		class ShaderBase;
		class TextureBase;
	}
	namespace PS4 {
		class RenderObjectPS4	{
		public:
			RenderObjectPS4();
			~RenderObjectPS4();

			RenderObjectPS4(MeshGeometry* m, Rendering::ShaderBase* shader, Rendering::TextureBase* tex);

			const Maths::Mat4& GetLocalTransform() const;
			void	SetLocalTransform(const Maths::Mat4& mat);

			Rendering::ShaderBase* GetShader() const {
				return shader;
			}


		protected:
			MeshGeometry*	mesh;
			Rendering::ShaderBase*		shader;

			static const int TEXTURE_COUNT = 4;
			Rendering::TextureBase* textures[TEXTURE_COUNT];

			Maths::Mat4		localTransform;
		};
	}
}
