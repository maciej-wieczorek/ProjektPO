#include "Entity.hpp"

#include <cassert>

Entity::Entity(int hitPoints) :
	mVelocity{},
	mFacingDirection{sf::Vector2f(1.f, 0.f)},
	mHitpoints{ hitPoints }
{
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

void Entity::setFacingDirection(sf::Vector2f direction)
{
	mFacingDirection = direction;
}

void Entity::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}

sf::Vector2f Entity::getFacingDirection() const
{
	return mFacingDirection;
}

int Entity::getHitpoints() const
{
	return mHitpoints;
}

void Entity::setHitpoints(int points)
{
	assert(points > 0);
	mHitpoints = points;
}

void Entity::heal(int points)
{
	assert(points > 0);
	mHitpoints += points;
}

void Entity::damage(int points)
{
	assert(points > 0);
	mHitpoints -= points;
}

void Entity::kill()
{
	mHitpoints = 0;
}

void Entity::remove()
{
	kill();
}

bool Entity::isKilled() const
{
	return mHitpoints <= 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	move(mVelocity * dt.asSeconds());
}
