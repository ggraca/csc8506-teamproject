#pragma once

#include "../Common/Vector4.h"
#include "../Common/Vector3.h"
#include "../Common/Quaternion.h"

using namespace NCL::Maths;

enum LightType {
	Point,
	Spot,
	Directional
};

class Light {
public:
	Light(LightType type, Vector3 position, Vector4 colour, float radius, float brightness, Quaternion orientation) {
		this->type = type;
		this->position = position;
		this->orientation = orientation;
		this->colour = colour;
		this->radius = radius;
		this->brightness = brightness;
	}
	Light() {};
	~Light(void) {};

	LightType GetType() const { return type; }
	void SetType(LightType val) { type = val; }

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }

	Quaternion GetOrientation() const { return orientation; }
	void SetOrientation(Quaternion val) { orientation = val; }

	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 val) { colour = val; }

	float GetBrightness() const { return brightness; }
	void SetBrightness(float val) { brightness = val; }

protected:
	LightType type;
	Vector3 position;
	Quaternion orientation;
	Vector4 colour;
	float radius;
	float brightness;
};