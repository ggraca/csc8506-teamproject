#pragma once

#include "Plane.h"
#include "Matrix4.h"
#include "Vector3.h"

namespace NCL {
	namespace Rendering {
		class Frustum {
		public:
			Frustum(void) {};
			~Frustum(void) {};

			void FromMatrix(const NCL::Maths::Matrix4& mvp);
			bool InsideFrustum(NCL::Maths::Vector3& position, float radius);

		protected:
			NCL::Maths::Plane planes[6];
		};
	}
}