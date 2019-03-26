#include "Plane.h"

using namespace NCL;
using namespace NCL::Maths;

Plane::Plane(const Vec3 &normal, float distance, bool normalise) {
	if(normalise) {
		float length = normal.Length();

		this->normal   = normal;
		this->normal.Normalise();

		this->distance = distance	/ length;
	}
	else{
		this->normal = normal;
		this->distance = distance;
	}
}

bool Plane::SphereInPlane(const Vec3 &position, float radius) const {
	if(Vec3::Dot(position,normal)+distance <= -radius) {
		return false;
	}
	return true;	
}

bool Plane::PointInPlane(const Vec3 &position) const {
	if(Vec3::Dot(position,normal)+distance < -0.001f) {
		return false;
	}

	return true;
}

Plane Plane::PlaneFromTri(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2) {
	Vec3 v1v0 = v1-v0;
	Vec3 v2v0 = v2-v0;

	Vec3 normal = Vec3::Cross(v1v0,v2v0);

	
	normal.Normalise();
	float d = -Vec3::Dot(v0,normal);
	return Plane(normal,d,false);
}

float	Plane::DistanceFromPlane(const Vec3 &in) const{
	return Vec3::Dot(in,normal)+distance;
}

Vec3 Plane::ProjectPointOntoPlane(const Vec3 &point) const {
	float distance = DistanceFromPlane(point);

	return point - (normal * distance);
}