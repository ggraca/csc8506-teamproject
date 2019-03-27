#pragma once
#include "../Common/Matrix4.h"
#include "../Common/Material.h"
#include "../Common/MeshGeometry.h"
#include "Component.h"


namespace NCL {
	using namespace NCL::Rendering;
	class MeshGeometry;

	namespace CSC8503 {
		class Transform;
		
		using namespace Maths;

		class RenderObject : virtual public Component
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

			float GetBoundingRadius() const { return boundingRadius; }
			void SetBoundingRadius(float radius) { boundingRadius = radius; }

		protected:
			MeshGeometry*	mesh;
			Material* material;
			Transform*		transform;

			float boundingRadius = 100.0f;

			bool materialInstanced = false;
		};
	}
}
