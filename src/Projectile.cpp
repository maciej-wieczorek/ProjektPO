#include "Projectile.hpp"
#include "DataTables.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

const std::vector<ProjectileData> Table = initializeProjectileData();

Projectile::Projectile(Type type, const TextureHolder& textures) :
	Entity{ 1 },
	mType{ type },
	mSprite{ textures.get(Table[static_cast<unsigned int>(type)].texture),
						  Table[static_cast<unsigned int>(type)].textureRect }
{
	centerOrigin(mSprite);
}

unsigned int Projectile::getCategory() const
{
	if (mType == Type::AlliedBullet)
		return static_cast<unsigned int>(Category::AlliedProjectile);
	else
		return static_cast<unsigned int>(Category::EnemyProjectile);
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
	return Table[static_cast<int>(mType)].speed;
}

int Projectile::getDamage() const
{
	return Table[static_cast<int>(mType)].damage;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// update projectiles position using Entity's method
	Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// draw projectile as a single sprite
	target.draw(mSprite, states);
}
