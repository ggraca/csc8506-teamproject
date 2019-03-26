/******************************************************************************
Class:Vec2
Implements:
Author:Rich Davison
Description:VERY simple Vec2 class. Students are encouraged to modify 
this as necessary!


-_-_-_-_-_-_-_,------,
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""

*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <iostream>

namespace NCL {
	namespace Maths {
		class Vec2 {

		public:
			Vec2(void) {
				ToZero();
			}

			Vec2(const float x, const float y) {
				this->x = x;
				this->y = y;
			}

			~Vec2(void) {}

			float x;
			float y;

			void ToZero() {
				x = 0.0f;
				y = 0.0f;
			}

			inline friend std::ostream& operator<<(std::ostream& o, const Vec2& v) {
				o << "Vec2(" << v.x << "," << v.y << ")" << std::endl;
				return o;
			}

			inline Vec2  operator-(const Vec2  &a) const {
				return Vec2(x - a.x, y - a.y);
			}

			inline Vec2  operator+(const Vec2  &a) const {
				return Vec2(x + a.x, y + a.y);
			}

			inline Vec2  operator/(const Vec2  &a) const {
				return Vec2(x / a.x, y / a.y);
			};

			inline Vec2  operator*(float f) const {
				return Vec2(x * f, y * f);
			};

			inline Vec2  operator/(float f) const {
				return Vec2(x / f, y / f);
			};

			inline void operator+=(const Vec2  &a) {
				x += a.x;
				y += a.y;
			}

			inline void operator-=(const Vec2  &a) {
				x -= a.x;
				y -= a.y;
			}

			inline void operator*=(float f) {
				x *= f;
				y *= f;
			}

			inline void operator/=(float f) {
				x /= f;
				y /= f;
			}
		};
	}
}
