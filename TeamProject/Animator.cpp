#include "Animator.h"



Animator::Animator()
{
}

Animator::Animator(GameObject * gameObject) : Component(gameObject)
{
}

Animator::~Animator()
{
	if (defaultAnimation) { delete defaultAnimation; defaultAnimation = nullptr;}
	if (currentAnimation) { delete currentAnimation; currentAnimation = nullptr; }
	ClearAnimations();
}

void Animator::SetDefaultAnimationState(AnimationState * anim)
{
	defaultAnimation = anim;
}

void Animator::SetCurrentAnimationState(AnimationState * anim)
{
	currentAnimation = anim;
	currentAnimation->animation->Play();
}

AnimationState * Animator::GetCurrentAnimationState()
{
	return currentAnimation;
}

void Animator::ResetAnimator()
{
	currentAnimation = defaultAnimation;
	currentAnimation->animation->Play();
}

void Animator::AddAnimationState(AnimationState * anim)
{
	animations.push_back(anim);

	if (animations.size() == 1)
	{
		defaultAnimation = animations[0];
		ResetAnimator();
	}
}

void Animator::Update(float dt)
{
	if (!currentAnimation) { return; }
	
	AnimationState* nextState = currentAnimation->ShouldStateChange(gameObject);
	if (!nextState) 
	{
		if (!currentAnimation->GetTargetObject())
		{
			currentAnimation->animation->UpdateAnimation(gameObject,dt);
		}
		else
		{
			currentAnimation->animation->UpdateAnimation(currentAnimation->GetTargetObject(), dt);
		}
		
		return; 
	}
	currentAnimation->animation->ResetAnimation();
	currentAnimation = nextState;
	currentAnimation->animation->Play();
}

void Animator::ClearAnimations()
{
	for (auto&i : animations)
	{
		delete i;
	}

	animations.clear();
}
