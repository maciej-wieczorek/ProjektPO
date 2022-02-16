#pragma once

#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <map>
#include <functional>

namespace sf
{
	class Event;
	class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
public:
	enum class Action
	{
		Push, Pop, Clear
	};

	explicit StateStack(State::Context context);

	template <class T>
	void registerState(States stateID);

	template <class T, class Param1>
	void registerState(States stateID, Param1 arg1);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void pushState(States stateID);
	void popState();
	void clearStates();

	bool isEmpty() const;

private:
	State::Ptr createState(States stateID);
	void applyPendingChanges();

	struct PendingChange
	{
		explicit PendingChange(Action action, States stateID = States::None);

		Action action;
		States stateID;
	};

	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;

	State::Context mContext;
	std::map<States, std::function<State::Ptr()>> mFactories;
};

template<class T>
inline void StateStack::registerState(States stateID)
{
	mFactories[stateID] = [this]()
	{
		return State::Ptr(new T(*this, mContext));
	};
}

template<class T, class Param1>
inline void StateStack::registerState(States stateID, Param1 arg1)
{
	mFactories[stateID] = [this, arg1]()
	{
		return new State::Ptr(new T(*this, mContext, arg1));
	};
}
