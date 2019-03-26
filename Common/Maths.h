#pragma once
#include <algorithm>

namespace NCL {
	namespace Maths {
		class Vec2;
		class Vec3;

		//It's pi(ish)...
		static const float		PI = 3.14159265358979323846f;

		//It's pi...divided by 360.0f!
		static const float		PI_OVER_360 = PI / 360.0f;

		//Radians to degrees
		inline float RadiansToDegrees(float degrees) {
			return degrees * 180.0f / PI;
		};

		//Degrees to radians
		inline float DegreesToRadians(float radians) {
			return radians * PI / 180.0f;
		};

		template<class T>
		inline float Clamp(T value, T min, T max) {
			if (value < min) {
				return min;
			}
			if (value > max) {
				return max;
			}
			return value;
		}

		Vec3 Clamp(const Vec3& a, const Vec3&mins, const Vec3& maxs);

		template<class T>
		inline T Lerp(const T& a, const T&b, float by) {
			return (a * (1.0f - by) + b*by);
		}

		void ScreenBoxOfTri(const Vec3& v0, const Vec3& v1, const Vec3& v2, Vec2& topLeft, Vec2& bottomRight);

		int ScreenAreaOfTri(const Vec3 &a, const Vec3 &b, const Vec3 & c);
		float FloatAreaOfTri(const Vec3 &a, const Vec3 &b, const Vec3 & c);

		float CrossAreaOfTri(const Vec3 &a, const Vec3 &b, const Vec3 & c);
	}
}