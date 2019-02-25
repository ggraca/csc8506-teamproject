#pragma once
#include "../Common/Matrix4.h"
#include "../Common/Material.h"

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

			void SetMaterialInstanced() {
				material = new Material(*material);
				materialInstanced = true;
			}

		protected:
			MeshGeometry*	mesh;
			Material* material;
			Transform*		transform;

			bool materialInstanced = false;
		};
	}
}
