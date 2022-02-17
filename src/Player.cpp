#include "Player.hpp"
#include "Character.hpp"
#include "Command.hpp"
#include "Utility.hpp"

Player::Player(unsigned int ID, const KeyBinding* binding) :
	mID{ ID },
	mKeyBinding{ binding },
	mActionBinding{}
{
	initializeActions();

	// set category of every action to PlayerCharacter
	for (auto& [action, command] : mActionBinding)
		command.category = static_cast<unsigned int>(Category::PlayerCharacter);
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		PlayerAction action;
		if (mKeyBinding->checkAction(event.key.code, action) && !isRealTimeAction(action))
		{
			commands.push(mActionBinding[action]);
		}
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	std::vector<PlayerAction> activeActions{ mKeyBinding->getRealtimeActions() };
	for (PlayerAction action : activeActions)
		commands.push(mActionBinding[action]);
}

unsigned int Player::getID()
{
	return mID;
}

struct CharacterMover
{
	CharacterMover(float vx, float vy, int identifier)
		: velocity(vx, vy)
		, characterID(identifier)
	{
	}

	void operator() (Character& character, sf::Time) const
	{
		if (character.getID() == characterID)
		{
			character.accelerate(velocity * character.getMaxSpeed());
			if (character.getVelocity() != sf::Vector2f(0.f, 0.f))
				character.setFacingDirection(unitVector(character.getVelocity()));
		}
	}

	sf::Vector2f velocity;
	int characterID;
};

struct CharacterFireTrigger
{
	CharacterFireTrigger(int identifier)
		: characterID(identifier)
	{
	}

	void operator() (Character& character, sf::Time) const
	{
		if (character.getID() == characterID)
			character.fire();
	}

	int characterID;
};

void Player::initializeActions()
{
	mActionBinding[PlayerAction::MoveLeft].action = derivedAction<Character>(CharacterMover(-1, 0, mID));
	mActionBinding[PlayerAction::MoveRight].action = derivedAction<Character>(CharacterMover(1, 0, mID));
	mActionBinding[PlayerAction::MoveUp].action = derivedAction<Character>(CharacterMover(0, -1, mID));
	mActionBinding[PlayerAction::MoveDown].action = derivedAction<Character>(CharacterMover(0, 1, mID));

	mActionBinding[PlayerAction::Fire].action = derivedAction<Character>(CharacterFireTrigger(mID));
}
