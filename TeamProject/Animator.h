#pragma once
#include "Component.h"
#include "Animation.h"
#include <vector>

class AnimationState;

class Transition
{
public:
	typedef bool(*TransitionFunction)(GameObject *);
	AnimationState * destinationState = nullptr;

	bool HasConditionSatisfied(GameObject * data)
	{
		if (!transitionFunction) { return false; }
		return transitionFunction(data);
	}

	TransitionFunction transitionFunction = nullptr;
};

class AnimationState
{
	
public:
	Animation * animation = nullptr;
	
	AnimationState(Animation * anim)
	{
		animation = anim;
	}
	
	AnimationState(Animation * anim, GameObject * target)
	{
		animation = anim;
		targetObject = target;
	}

	~AnimationState()
	{
		for (auto&i : transitions)
		{
			delete i;
		}
		transitions.clear();
		if (animation) { delete animation; }
	}

	void AddTransition(Transition * transition)
	{
		transitions.push_back(transition);
	}

	AnimationState* ShouldStateChange(GameObject * obj)
	{
		if (!obj) { return nullptr; }

		for (auto&i : transitions)
		{
			if (i->HasConditionSatisfied(obj))
			{
				return i->destinationState;
			}
		}
		return nullptr;
	}

	void SetTargetObject(GameObject * target)
	{
		targetObject = target;
	}

	GameObject * GetTargetObject()
	{
		return targetObject;
	}

protected:
	std::vector<Transition*> transitions;
	GameObject * targetObject = nullptr;
};


class Animator: public Component
{
public:
	Animator();
	Animator(GameObject * gameObject);
	~Animator();
	
	void SetDefaultAnimationState(AnimationState * anim);
	void SetCurrentAnimationState(AnimationState * anim);
	AnimationState * GetCurrentAnimationState();
	void ResetAnimator();
	void AddAnimationState(AnimationState * anim);
	void Update(float dt) override;

protected:
	void ClearAnimations();

	AnimationState * defaultAnimation = nullptr;
	AnimationState * currentAnimation = nullptr;
	vector<AnimationState*> animations;
};

