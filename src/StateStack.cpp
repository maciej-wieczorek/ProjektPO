#include "StateStack.hpp"

#include <cassert>

StateStack::StateStack(State::Context context) :
	mStack{},
	mPendingList{},
	mContext{ context },
	mFactories{}
{
}

void StateStack::update(sf::Time dt)
{
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	// draw from bottom to top so that underlying states are visable
	for (State::Ptr& state : mStack)
		state->draw();
}

void StateStack::handleEvent(const sf::Event& event)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::pushState(States stateID)
{
	mPendingList.push_back(PendingChange(Action::Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Action::Push: // create new state from stateID and add it to the stack
			mStack.push_back(createState(change.stateID));
			break;

		case Action::Pop: // destroy current state and activate a new one
			mStack.back()->onDestroy();
			mStack.pop_back();

			if (!mStack.empty())
				mStack.back()->onActivate();
			break;
			
		case Action::Clear: // destroy all states and clear the stack
			for (State::Ptr& state : mStack)
				state->onDestroy();

			mStack.clear();
			break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States stateID) :
	action{ action },
	stateID{ stateID }
{
}
