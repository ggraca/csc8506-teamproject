#pragma once
#include "../Common/Matrix4.h"
#include "../TeamProject/Material.h"

namespace NCL {
	using namespace NCL::Rendering;
	class MeshGeometry;
	namespace CSC8503 {
		class Transform;
		using namespace Maths;

		class RenderObject
		{
		public:
			RenderObject(Transform* parentTransform, MeshGeometry* mesh, Material* material);
			~RenderObject();

			Material* GetMaterial() const { return material; }
			void SetMaterial(Material* m) { material = m; }

			MeshGeometry*	GetMesh() const {
				return mesh;
			}

			Transform*		GetTransform() const {
				return transform;
			}

			void SetColour(const Vector4& c) {
				colour = c;
			}

			Vector4 GetColour() const {
				return colour;
			}

		protected:
			MeshGeometry*	mesh;
			Material* material;
			Transform*		transform;
			Vector4			colour;
		};
	}
}
