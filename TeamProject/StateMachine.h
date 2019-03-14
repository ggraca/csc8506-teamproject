#pragma once
#include <vector>
#include <map>

class State;
class StateTransition;

typedef std::multimap<State*, StateTransition*> TransitionContainer;
typedef TransitionContainer::iterator TransitionIterator;

class StateMachine {
public:
	StateMachine();
	~StateMachine();

	void AddState(State* s);
	void AddTransition(StateTransition* t);

	void SetActiveState(State* state) { activeState = state; }

	void Update();

protected:
	State* activeState;

	std::vector<State*> allStates;

	TransitionContainer allTransitions;
};
