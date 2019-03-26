#include "Maths.h"
#include "../Common/Vector2.h"
#include "../Common/Vector3.h"

namespace NCL {
	namespace Maths {
		void ScreenBoxOfTri(const Vec3& v0, const Vec3& v1, const Vec3& v2, Vec2& topLeft, Vec2& bottomRight) {
			topLeft.x = std::min(v0.x, std::min(v1.x, v2.x));
			topLeft.y = std::min(v0.y, std::min(v1.y, v2.y));

			bottomRight.x = std::max(v0.x, std::max(v1.x, v2.x));
			bottomRight.y = std::max(v0.y, std::max(v1.y, v2.y));
		}

		int ScreenAreaOfTri(const Vec3 &a, const Vec3 &b, const Vec3 & c) {
			int area =(int) (((a.x * b.y) + (b.x * c.y) + (c.x * a.y)) -
				((b.x * a.y) + (c.x * b.y) + (a.x * c.y)));
			return (area >> 1);
		}

		float FloatAreaOfTri(const Vec3 &a, const Vec3 &b, const Vec3 & c) {
			float area = ((a.x * b.y) + (b.x * c.y) + (c.x * a.y)) -
				((b.x * a.y) + (c.x * b.y) + (a.x * c.y));
			return (area * 0.5f);
		}

		float CrossAreaOfTri(const Vec3 &a, const Vec3 &b, const Vec3 & c) {
			Vec3 area = Vec3::Cross(a - b, a - c);
			return area.Length() * 0.5f;
		}
	

		Vec3 Clamp(const Vec3& a, const Vec3&mins, const Vec3& maxs) {
			return Vec3(
				Clamp(a.x, mins.x, maxs.x),
				Clamp(a.y, mins.y, maxs.y),
				Clamp(a.z, mins.z, maxs.z)
			);
		}
	}
}