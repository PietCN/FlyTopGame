#pragma once
#ifndef AIRCRAFT_GUARD
#define AIRCRAFT_GUARD

#include <Entity.hpp>
#include <ResourceIdentifier.hpp>
#include <Commandqueue.hpp>
#include <Command.hpp>

#include <Projectile.hpp>
#include <Pickup.hpp>

class TextNode;

class Aircraft : public Entity
{
public:
	enum Type
	{ 
		Eagle, 
		Raptor, 
		Avenger, 
		TypeCount 
	};
	
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);

	virtual size_t		getCategory() const;

public:
	explicit Aircraft::Aircraft(Type type, const TextureHolder& texture, const FontHolder& fonts);

	void							fire();
	void							launchMissle();

	void							collectMissiles(size_t missleAmmo);
	void							increasSpread();
	void							increasFireRate();

	virtual bool			isMarkedForRemoval() const;

	sf::FloatRect			getBoundingRect() const;

private:
	void							updateMovementPattern(sf::Time dt);
	float							getMaxSpeed();

	bool							isAllied() const;

	void							checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
	void							checkPickupCommand(CommandQueue& command);
	void							updateTexts();

	void							createBullets(SceneNode& node, const TextureHolder& textures) const;
	void							createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
	void							createPickup(SceneNode& node, const TextureHolder& texture) const;

private:
	Type							mType;
	sf::Sprite				mSprite;
	TextNode*					mHealtDisplay;
	TextNode*					mMissleDisplay;
	float							mTraveledDistance;
	float							mFireRateLevel;

	std::size_t				mDirectionIndex;
	bool							mIsFiring;
	bool							mIsLaunchMissile;
	bool							mIsMarkedForRemoval;
	size_t						mMissileAmmo;
	size_t						mSpreadLevel;
	sf::Time					mFireCoundDown;

	Command						mFireCommand;
	Command						mMissleCommand;
	Command						mPickupCommand;

};

#endif AIRCRAFT_GUARD