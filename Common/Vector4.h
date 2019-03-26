/*
Class:Vec4
Implements:
Author:Rich Davison
Description:VERY simple Vec4 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#pragma once

namespace NCL {
	namespace Maths {
		class Vec4 {
		public:
			Vec4(void) {
				x = y = z = w = 1.0f;
			}
			Vec4(float x, float y, float z, float w) {
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
			}

			inline Vec3 ToVector3() {
				return Vec3(x, y, z);
			}

			~Vec4(void) {}

			union {
				struct {
					float x;
					float y;
					float z;
					float w;
				};
				float array[4];
			};

			inline Vec4  operator*(const float a) const {
				return Vec4(x * a, y * a, z * a, w * a);
			}

			inline Vec4  operator/(const float a) const {
				float r = 1.0f / a;
				return Vec4(x * r, y * r, z * r, w * r);
			}

			inline Vec4  operator+(const Vec4  &a) const {
				return Vec4(x + a.x, y + a.y, z + a.z, w + a.w);
			}

			inline Vec4  operator-(const Vec4  &a) const {
				return Vec4(x - a.x, y - a.y, z - a.z, w - a.w);
			}

			inline void operator+=(const Vec4  &a) {
				x += a.x;
				y += a.y;
				z += a.z;
				w += a.w;
			}

			inline void operator-=(const Vec4  &a) {
				x -= a.x;
				y -= a.y;
				z -= a.z;
				w -= a.w;
			}


			inline void operator*=(float f) {
				x *= f;
				y *= f;
				z *= f;
				w *= f;
			}

			inline void operator/=(float f) {
				float r = 1.0f / f;
				x *= r;
				y *= r;
				z *= r;
				w *= r;
			}

			inline void operator+=(float f) {
				x += f;
				y += f;
				z += f;
				w += f;
			}

			inline void operator-=(float f) {
				x -= f;
				y -= f;
				z -= f;
				w -= f;
			}
		};
	}
}