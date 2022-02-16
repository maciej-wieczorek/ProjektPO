#pragma once

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity
{
public:

	enum class Type
	{
		AlliedBullet = 0,
		EnemyBullet,
		TypeCount // this keeps track how many types there are
	};

	Projectile(Type type, const TextureHolder& textures);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	float getMaxSpeed() const;
	int getDamage() const;

private:

	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	Type mType;
	sf::Sprite mSprite;
};

