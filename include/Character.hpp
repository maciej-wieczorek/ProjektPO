#pragma once

#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Projectile.hpp"
#include "TextNode.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include <string>

class Character : public Entity
{
public:
	enum class Type
	{
		Character1,
		Character2,
		Enemy1,
		Enemy2,
		TypeCount
	};

	Character(Type type, const TextureHolder& textures, const FontHolder& fonts);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual void remove();
	virtual bool isMarkedForRemoval() const;
	bool isAllied() const;
	float getMaxSpeed() const;
	void disablePickups();
	void increaseFireRate();
	void fire();

	unsigned int getID() const;
	void setID(int identifier);

	operator std::string() const;

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

	void checkPickupDrop(CommandQueue& commands);
	void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
		 
	void createBullets(SceneNode& node, const TextureHolder& textures) const;
	void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
	void createPickup(SceneNode& node, const TextureHolder& textures) const;
		 
	void updateTexts();
	void updateWalkAnimation();


	Type mType;
	sf::Sprite mSprite;
	Command mFireCommand;
	sf::Time mFireCountdown;
	bool mSpawnedPickup; // enemys spawn pickups
	bool mPickupsEnabled;
	bool mIsFiring;
	int mFireRateLevel;
	Command mDropPickupCommand;
	TextNode* mHealthDisplay;
	unsigned int mID;
};

