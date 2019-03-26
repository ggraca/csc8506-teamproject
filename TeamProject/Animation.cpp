#include "Animation.h"
#include "PhysicsObject.h"
#include "../Plugins/Bullet/src/btBulletDynamicsCommon.h"

Animation::Animation(int fps)
{
	currentFrame = nullptr;
	nextFrame = nullptr;
	interpolation = nullptr;
	currentTime = -1;
	currentIndex = -1;
	targetFPS = fps;
}


Animation::~Animation()
{
	if (currentFrame) { delete currentFrame; currentFrame = nullptr; }
	if (nextFrame)	  { delete nextFrame;nextFrame = nullptr; }
	if(interpolation) { delete interpolation;interpolation = nullptr; }

	ClearKeyFrames();
}

void Animation::AddKeyFrame(KeyFrame * keyFrame)
{
	for (unsigned int i = 0; i < keyFrames.size(); i++)
	{
		if (keyFrames[i]->time > keyFrame->time)
		{
			keyFrames.insert(keyFrames.begin() + i, keyFrame);
			return;
		}
	}

	keyFrames.push_back(keyFrame);
}

void Animation::Play()
{
	hasAnimationStarted = true;
}

void Animation::Stop()
{
	hasAnimationStarted = false;
}

void Animation::UpdateAnimation(GameObject * obj, float dt)
{
	if (!hasAnimationStarted) { return; }
	if (keyFrames.size() == 0) { return; }

	if (currentTime < 0)
	{
		SetupFirstFrameIterations(obj);

		CalculateInterpolation();

	}
	else if (currentTime > nextFrame->time)
	{
		currentIndex++;

		if ((int)keyFrames.size() <= currentIndex+1) { hasAnimationFinished = true; return; }

		ShiftFrames(obj);
		CalculateInterpolation();
	}

	UpdateObjectTransform(obj);

	currentTime += dt;
}

void Animation::UpdateObjectTransform(GameObject * obj)
{
	obj->GetTransform().ForceUpdateScaleWithTransform(obj->GetTransform().GetLocalScale()+interpolation->localScale);
	obj->GetTransform().ForceUpdateLocalPositionWithTransform(obj->GetTransform().GetLocalPosition()+interpolation->localPosition);
	auto currentRotation = obj->GetTransform().GetLocalOrientation().ToEuler();
	currentRotation += interpolation->localRotation;
	obj->GetTransform().ForceUpdateLocalRotationWithTransform(Quaternion::EulerAnglesToQuaternion(currentRotation.x, currentRotation.y, currentRotation.z));
}

void Animation::CalculateInterpolation()
{

	if (interpolation) { delete interpolation; interpolation = nullptr; }

	interpolation = new KeyFrame();

	float frameTimeDifference = nextFrame->time - currentFrame->time;
	float frameCountDifference = (targetFPS * frameTimeDifference);

	Vec3 positionDifferencePerFrame = (nextFrame->localPosition - currentFrame->localPosition) / frameCountDifference;
	Vec3 scaleDifferencePerFrame = (nextFrame->localScale - currentFrame->localScale) / frameCountDifference;
	Vec3 rotationDifferencePerFrame = (nextFrame->localRotation - currentFrame->localRotation) / frameCountDifference;


	interpolation->localPosition = positionDifferencePerFrame;
	interpolation->localScale = scaleDifferencePerFrame;
	interpolation->localRotation = rotationDifferencePerFrame;
}

void Animation::ShiftFrames(GameObject * obj)
{
	currentFrame = keyFrames[currentIndex];

	if (currentFrame->keyFunction)
	{
		currentFrame->keyFunction(obj);
	}

	nextFrame = keyFrames[currentIndex + 1];
}

void Animation::SetupFirstFrameIterations(GameObject * obj)
{
	currentTime = 0;
	currentFrame = new KeyFrame();
	currentFrame->localPosition = obj->GetTransform().GetLocalPosition();
	currentFrame->localRotation = obj->GetTransform().GetLocalOrientation().ToEuler();
	currentFrame->localScale = obj->GetTransform().GetLocalScale();

	nextFrame = keyFrames[currentIndex + 1];
}

void Animation::ResetAnimation()
{
	currentFrame = nullptr;
	nextFrame = nullptr;
	interpolation = nullptr;
	currentTime = -1;
	currentIndex = -1;
	hasAnimationStarted = false;
	hasAnimationFinished = false;
}

void Animation::ClearKeyFrames()
{
	for (auto&i : keyFrames)
	{
		delete i;
	}
	keyFrames.clear();
}
