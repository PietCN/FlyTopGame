#pragma once
#ifndef PICKUP_GUARD
#define PICKUP_GUARD

#include <Entity.hpp>
#include <ResourceIdentifier.hpp>

class Aircraft;

class Pickup : public Entity
{
public:
	enum Type
	{
		HealthRefill,
		MissileRefill,
		FireSpread,
		FireRate,
		TypeCount
	};

public:
	Pickup::Pickup(Type type, const TextureHolder& texture);

	virtual size_t					getCategory() const;
	virtual sf::FloatRect		getBoundingRect() const;

	void										apply(Aircraft& player) const;

protected:
	virtual void						drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type										mType;
	sf::Sprite							mSprite;
};




#endif PICKUP_GUARD