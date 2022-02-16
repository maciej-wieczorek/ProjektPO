#pragma once

#include "SceneNode.hpp"
#include "CommandQueue.hpp"

class Entity : public SceneNode
{
public:
	explicit Entity(int hitPoints); // aka healthPoints

	void setVelocity(sf::Vector2f velocity);
	void setVelocity(float vx, float vy);
	void setFacingDirection(sf::Vector2f direction);
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);
	sf::Vector2f getVelocity() const;
	sf::Vector2f getFacingDirection() const;

	int	getHitpoints() const;
	void setHitpoints(int points);
	void heal(int points);
	void damage(int points);
	void kill();
	virtual void remove();
	virtual bool isKilled() const;

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	sf::Vector2f mVelocity;
	sf::Vector2f mFacingDirection;
	int mHitpoints;
};

