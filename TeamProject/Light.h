#pragma once

#include "Component.h"
#include "Transform.h"
#include "../Common/Vector4.h"


enum LightType {
	Directional,
	Point,
	Spot
};

class Light : public Component {
public:
	Light(LightType type, NCL::Maths::Vec4 colour, float radius, float brightness) {
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

	NCL::Maths::Vec4 GetColour() const { return colour; }
	void SetColour(NCL::Maths::Vec4 val) { colour = val; }

	float GetBrightness() const { return brightness; }
	void SetBrightness(float val) { brightness = val; }

protected:
	LightType type;
	NCL::Maths::Vec4 colour;
	float radius;
	float brightness;
};