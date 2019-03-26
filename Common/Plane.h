/******************************************************************************
Class:Plane
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:VERY simple Plane class. Students are encouraged to modify 
this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "vector3.h"
namespace NCL {
	namespace Maths {
		class Plane {
		public:
			Plane(void) {};
			Plane(const Vec3 &normal, float distance, bool normalise = false);

			~Plane(void) {};

			//Sets the planes normal, which should be UNIT LENGTH!!!
			void	SetNormal(const Vec3 &normal) { this->normal = normal; }
			//Gets the planes normal.
			Vec3 GetNormal() const { return normal; }
			//Sets the planes distance from the origin
			void	SetDistance(float dist) { distance = dist; }
			//Gets the planes distance from the origin
			float	GetDistance() const { return distance; }
			//Performs a simple sphere / plane test
			bool SphereInPlane(const Vec3 &position, float radius) const;
			//Performs a simple sphere / point test
			bool PointInPlane(const Vec3 &position) const;

			float	DistanceFromPlane(const Vec3 &in) const;

			Vec3 GetPointOnPlane() const {
				return normal * -distance;
			}

			Vec3 ProjectPointOntoPlane(const Vec3 &point) const;


			static Plane PlaneFromTri(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2);

		protected:
			//Unit-length plane normal
			Vec3 normal;
			//Distance from origin
			float	distance;
		};
	}
}