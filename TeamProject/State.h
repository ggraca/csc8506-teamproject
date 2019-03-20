#pragma once
#include <functional>
//#include "GameScene.h"

//class GameState;

class State {
public:
	State() {}
	virtual ~State() {}
	virtual void Update() = 0; //Pure virtual base class
};

typedef void(*StateFunc)(void*);

class GenericState : public State {
public:
	GenericState(StateFunc someFunc, void* someData) {
		func = someFunc;
		funcData = someData;
	}
	virtual void Update() {
		if (funcData != nullptr) {
			func(funcData);
		}
	}

protected:
	StateFunc func;
	void* funcData;
};

//class GameState;


template <class A>
class WorldState : public State {
public:
	typedef std::function <void(A*, void *)> WorldFunc;
	WorldState(WorldFunc someFunc, void* someData, A* someClass) {
		func = someFunc;
		funcData = someData;
		funcClass = someClass;
	}
	virtual void Update() {
		if (funcData != nullptr) {
			func(funcClass, funcData);
		}
	}
protected:
	WorldFunc func;
	void* funcData;
	A* funcClass;
};

//class GameState;

template <class A>
class MenuState : public State {
public:
	typedef std::function <void(A*)> MenuFunc;
	MenuState(MenuFunc someFunc, A* someClass)
	{
		func = someFunc;
		funcClass = someClass;

	}
	virtual void Update() {
		func(funcClass);
	}

protected:
	MenuFunc func;
	A* funcClass;
};
