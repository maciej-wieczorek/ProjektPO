#include "KeyBinding.hpp"

KeyBinding::KeyBinding(int preConfig) :
	mKeyMap{}
{
	if (preConfig == 1)
	{
		mKeyMap[sf::Keyboard::A] = PlayerAction::MoveLeft;
		mKeyMap[sf::Keyboard::D] = PlayerAction::MoveRight;
		mKeyMap[sf::Keyboard::W] = PlayerAction::MoveUp;
		mKeyMap[sf::Keyboard::S] = PlayerAction::MoveDown;
		mKeyMap[sf::Keyboard::Space] = PlayerAction::Fire;
		mKeyMap[sf::Keyboard::H] = PlayerAction::WeaponSwitchUp;
		mKeyMap[sf::Keyboard::G] = PlayerAction::WeaponSwitchDown;
	}
	else if (preConfig == 2)
	{
		mKeyMap[sf::Keyboard::Left] = PlayerAction::MoveLeft;
		mKeyMap[sf::Keyboard::Right] = PlayerAction::MoveRight;
		mKeyMap[sf::Keyboard::Up] = PlayerAction::MoveUp;
		mKeyMap[sf::Keyboard::Down] = PlayerAction::MoveDown;
		mKeyMap[sf::Keyboard::RShift] = PlayerAction::Fire;
		mKeyMap[sf::Keyboard::RBracket] = PlayerAction::WeaponSwitchUp;
		mKeyMap[sf::Keyboard::LBracket] = PlayerAction::WeaponSwitchDown;
	}
}

void KeyBinding::assignKey(PlayerAction action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyMap.begin(); itr != mKeyMap.end(); )
	{
		if (itr->second == action)
			mKeyMap.erase(itr++);
		else
			itr++;
	}

	// Insert new binding
	mKeyMap[key] = action;
}

sf::Keyboard::Key KeyBinding::getAssignedKey(PlayerAction action) const
{
	for (auto [mappedKey, mappedAction] : mKeyMap)
	{
		if (mappedAction == action)
			return mappedKey;
	}

	return sf::Keyboard::Unknown;
}

bool KeyBinding::checkAction(sf::Keyboard::Key key, PlayerAction& out) const
{
	std::map<sf::Keyboard::Key, PlayerAction>::const_iterator found = mKeyMap.find(key);
	if (found == mKeyMap.end())
	{
		return false;
	}
	else
	{
		out = found->second;
		return true;
	}
}

std::vector<PlayerAction> KeyBinding::getRealtimeActions() const
{
	std::vector<PlayerAction> actions;

	for (auto [key, action] : mKeyMap)
	{
		if (sf::Keyboard::isKeyPressed(key) && isRealTimeAction(action))
			actions.push_back(action);
	}

	return actions;
}

bool isRealTimeAction(PlayerAction action)
{
	switch (action)
	{
	case PlayerAction::MoveLeft:
	case PlayerAction::MoveRight:
	case PlayerAction::MoveUp:
	case PlayerAction::MoveDown:
	case PlayerAction::Fire:
	case PlayerAction::WeaponSwitchUp:
	case PlayerAction::WeaponSwitchDown:
		return true;

	default:
		return false;
	}
}