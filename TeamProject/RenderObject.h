#pragma once
#include "../Common/Matrix4.h"
#include "../Common/Material.h"

namespace NCL {
	using namespace NCL::Rendering;
	class MeshGeometry;
	namespace CSC8503 {
		class Component;
		class Transform;
		using namespace Maths;

		class RenderObject:virtual public Component
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

		protected:
			MeshGeometry*	mesh;
			Material* material;
			Transform*		transform;
		};
	}
}
