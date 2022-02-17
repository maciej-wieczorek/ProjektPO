#pragma once

#include "CommandQueue.hpp"
#include "KeyBinding.hpp"

#include <SFML/Window/Event.hpp>

#include <map>

class Player
{
public:
	Player(unsigned int ID, const KeyBinding* binding);

	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);
	unsigned int getID();

private:
	void initializeActions();

	unsigned int mID; // Player's ID
	const KeyBinding* mKeyBinding; // this maps keys to actions
	std::map<PlayerAction, Command> mActionBinding; // this maps actions to commands
};

