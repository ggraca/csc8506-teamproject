#pragma once

#include "Component.h"
#include "Transform.h"

using namespace NCL::Maths;

enum LightType {
	Point,
	Spot,
	Directional
};

class Light : public Component {
public:
	Light(Transform* transform, Vector4 colour, float radius, float brightness, Quaternion orientation) {
		this->type = type;
		this->transform = transform;
		this->colour = colour;
		this->radius = radius;
		this->brightness = brightness;
	}
	Light() {};
	~Light(void) {};

	LightType GetType() const { return type; }
	void SetType(LightType val) { type = val; }

	Transform* GetTransform() const { return transform; }

	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 val) { colour = val; }

	float GetBrightness() const { return brightness; }
	void SetBrightness(float val) { brightness = val; }

protected:
	LightType type;
	Transform* transform;
	Vector4 colour;
	float radius;
	float brightness;
};