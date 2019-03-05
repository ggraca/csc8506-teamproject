#pragma once

#include "Component.h"
#include "Transform.h"
#include "../Common/Vector4.h"

using namespace NCL::Maths;

enum LightType {
	Point,
	Spot,
	Directional
};

class Light : public Component {
public:
	Light(LightType type, Vector4 colour, float radius, float brightness) {
		this->type = type;
		this->colour = colour;
		this->radius = radius;
		this->brightness = brightness;
	}
	Light() {};
	~Light(void) {};

	LightType GetType() const { return type; }
	void SetType(LightType val) { type = val; }

	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 val) { colour = val; }

	float GetBrightness() const { return brightness; }
	void SetBrightness(float val) { brightness = val; }

protected:
	LightType type;
	Vector4 colour;
	float radius;
	float brightness;
};