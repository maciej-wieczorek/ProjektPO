#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures,
	FontHolder& fonts, KeyBinding& keys1, KeyBinding& keys2) :
	window{ &window },
	textures{ &textures },
	fonts{ &fonts },
	keys1{ &keys1 },
	keys2{ &keys2 }
{
}

State::State(StateStack& stack, Context context) :
	mStack{ &stack },
	mContext{ context }
{
}

State::~State()
{
	
}

void State::onActivate()
{
}

void State::onDestroy()
{
}

void State::requestStackPush(States stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}
