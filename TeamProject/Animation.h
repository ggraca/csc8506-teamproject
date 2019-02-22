#pragma once
#include "GameObject.h"
#include <vector>

using namespace NCL;
using namespace CSC8503;

typedef void(*KeyFrameFunction)(GameObject* obj);

struct KeyFrame
{
	float time;
	Vector3 localPosition;
	Vector3 localScale;
	Vector3 localRotation;
	KeyFrameFunction keyFunction = nullptr;
};

class Animation
{
public:
	Animation(int fps);
	~Animation();
	void AddKeyFrame(KeyFrame* keyFrame);
	void Play();
	void Stop();
	void UpdateAnimation(GameObject * obj, float dt);
	void UpdateObjectTransform(GameObject * obj);
	void CalculateInterpolation();
	void ShiftFrames(GameObject * obj);
	void SetupFirstFrameIterations(GameObject * obj);
protected:
	vector<KeyFrame*> keyFrames;
	KeyFrame* currentFrame;
	KeyFrame* nextFrame;
	KeyFrame* interpolation;
	int targetFPS;
	float currentTime;
	int currentIndex;
	bool hasAnimationStarted = false;
	bool hasAnimationFinished = false;

	void ClearKeyFrames();
};

