#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <vector>

enum class PlayerAction
{
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,
	Fire,
	WeaponSwitchUp,
	WeaponSwitchDown,
};

bool isRealTimeAction(PlayerAction action);


class KeyBinding
{
public:
	explicit KeyBinding(int preConfig = 1);

	void assignKey(PlayerAction action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(PlayerAction action) const;

	// check if key maps to action, if it does set "out" to mapped action
	bool checkAction(sf::Keyboard::Key key, PlayerAction& out) const;

	// returns all currently active real time actions
	std::vector<PlayerAction> getRealtimeActions() const;

private:
	std::map<sf::Keyboard::Key, PlayerAction> mKeyMap;
};