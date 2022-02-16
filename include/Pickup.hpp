#pragma once

#include "Entity.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Character;

class Pickup : public Entity
{
public:
	enum class Type
	{
		HealthRefill,
		FireRate,
		TypeCount
	};

	Pickup(Type type, const TextureHolder& textures);

	virtual unsigned int getCategory() const override;
	virtual sf::FloatRect getBoundingRect() const override;

	void apply(Character& player) const;

protected:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type mType;
	sf::Sprite mSprite;
};

