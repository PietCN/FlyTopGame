#pragma once
#ifndef ENTITY_GUARD
#define ENTITY_GUARD

#include <SceneNode.hpp>

class Entity : public SceneNode
{
public:
	explicit				Entity::Entity(int hitpoints);

	void						repair(int points);
	void						damage(int points);
	void						destroy();

	int							getHitpoints() const;
	virtual bool		isDestroyed() const;
	virtual bool		isMarkedForRemoval() const;

	void						setVelocity(sf::Vector2f velocity);
	void						setVelocity(float vX, float vY);
	void						accelerate(sf::Vector2f velocity);
	void						accelerate(float vX, float vY);

	sf::Vector2f		getVelocity() const;

protected:
	virtual void		updateCurrent(sf::Time dt, CommandQueue& commands);

private:
	sf::Vector2f		mVelocity;
	int							mHitpoints;
};

#endif ENTITY_GUARD