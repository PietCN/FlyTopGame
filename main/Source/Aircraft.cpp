#include <Aircraft.hpp>
#include <Projectile.hpp>
#include <Pickup.hpp>

#include <ResourceHolder.hpp>
#include <DataTables.hpp>
#include <TextNode.hpp>
#include <Utility.hpp>

#include <memory>
#include <cmath>

namespace
{
	const std::vector<AircraftData> Table = initializeAircraftData();
}

/*********************************************************************************
* Objekt Aircraft Cstr
* initialisiert den Typ des Objektes
*********************************************************************************/
Aircraft::Aircraft(Type type, const TextureHolder& texture, const FontHolder& fonts)
	: mType(type)
	, mIsFiring(false)
	, mIsLaunchMissile(false)
	, mIsMarkedForRemoval(false)
	, mMissileAmmo(2)
	, mFireRateLevel(1)
	, mFireCoundDown(sf::Time::Zero)
	, Entity(Table[type].hitpoints)
	, mSprite(texture.get(Table[type].texture), Table[type].textureRect)
	, mDirectionIndex(0)
	, mTraveledDistance(0.f)
	, mSpreadLevel(1)
	, mMissleDisplay(nullptr)
	, mHealtDisplay(nullptr)
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	std::unique_ptr<TextNode> healtDisplay(new TextNode(fonts, ""));
	mHealtDisplay = healtDisplay.get();
	attachChild(std::move(healtDisplay));

	if (type == Type::Eagle)
	{
		std::unique_ptr<TextNode> missleDisplay(new TextNode(fonts, ""));
		mMissleDisplay = missleDisplay.get();
		missleDisplay->setPosition(0.f, 70.f);
		attachChild(std::move(missleDisplay));
	}

	mPickupCommand.category = Category::SceneAirLayer;
	mPickupCommand.action =
		[this, &texture](SceneNode& node, sf::Time)
	{
		createPickup(node, texture);
	};

	mFireCommand.category = Category::SceneAirLayer;
	mFireCommand.action = 
		[this, &texture](SceneNode& node, sf::Time)
	{
		createBullets(node, texture);
	};

	mMissleCommand.category = Category::SceneAirLayer;
	mMissleCommand.action =
		[this, &texture] (SceneNode& node, sf::Time)
	{
		createProjectile(node, Projectile::Missile, 0.f, 0.5f, texture);
	};
	
}

/*********************************************************************************
* Draw-Methode für das Aircraft-Objekt
* 
*********************************************************************************/
void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isDestroyed())
	{
		checkPickupCommand(commands);

		mIsMarkedForRemoval = true;
		return;
	}

	updateMovementPattern(dt);
	checkProjectileLaunch(dt, commands);

	Entity::updateCurrent(dt, commands);
	updateTexts();
}

/*********************************************************************************
* Gibt die Kategory des Knotens zurück
*
*********************************************************************************/
size_t Aircraft::getCategory() const
{
	if (isAllied())
		return Category::PlayerAircraft;
	else
		return Category::EnemyAircraft;
}

/*********************************************************************************
* Gibt die Geschwindigkeit des benutzten Types zurück
*
*********************************************************************************/
float Aircraft::getMaxSpeed()
{
	return Table[mType].speed;
}

/*********************************************************************************
* Abarbeitung der Directions-Liste
*
*********************************************************************************/
void Aircraft::updateMovementPattern(sf::Time dt)
{
	const std::vector<Direction>& directions = Table[mType].directions;

	if (!directions.empty())
	{
		float distanceToTravel = directions[mDirectionIndex].distance;
		if (mTraveledDistance > distanceToTravel)
		{
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTraveledDistance = 0.f;
		}

		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);
		mTraveledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

/*********************************************************************************
* 
*
*********************************************************************************/
void Aircraft::fire()
{
	if (Table[mType].fireInterval != sf::Time::Zero)
		if (mFireCoundDown <= sf::Time::Zero)
			mIsFiring = true;
}

/*********************************************************************************
* 
*
*********************************************************************************/
void Aircraft::launchMissle()
{
	if (mMissileAmmo > 0)
	{
		mIsLaunchMissile = true;
		--mMissileAmmo;
	}
}

/*********************************************************************************
*
*
*********************************************************************************/
void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (mIsFiring)
	{
		commands.push(mFireCommand);
		mFireCoundDown = sf::seconds(1.f / (mFireRateLevel + 1));
		mIsFiring = false;
	}
	else if (mFireCoundDown > sf::Time::Zero)
	{
		mFireCoundDown -= dt;
	}
	if (mIsLaunchMissile)
	{
		commands.push(mMissleCommand);
		mIsLaunchMissile = false;
	}

	if (!isAllied())
		fire();
}

/*********************************************************************************
*
*
*********************************************************************************/
bool Aircraft::isAllied() const
{
	return mType == Type::Eagle;
}

/*********************************************************************************
*
*
*********************************************************************************/
void Aircraft::updateTexts()
{
	if (mMissleDisplay)
		if (isDestroyed())
			mMissleDisplay->setString("");
		else
			mMissleDisplay->setString("M:" + toString(mMissileAmmo));

	mHealtDisplay->setString(std::to_string(getHitpoints()) + " HP");
	mHealtDisplay->setPosition(0.f, 50.f);
	mHealtDisplay->setRotation(-getRotation());
}

/*********************************************************************************
*
*
*********************************************************************************/
void Aircraft::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

	switch (mSpreadLevel)
	{
	case 1:
		createProjectile(node, type, 0.0f, 0.5f, textures);
		break;

	case 2: 
		createProjectile(node, type, -0.33f, 0.33f, textures);
		createProjectile(node, type, +0.33f, 0.33f, textures);
		break;

	case 3:
		createProjectile(node, type, -0.5f, 0.33f, textures);
		createProjectile(node, type,  0.0f, 0.5f, textures);
		createProjectile(node, type, +0.5f, 0.33f, textures);
		break;

	}
}

/*********************************************************************************
*
*
*********************************************************************************/
void Aircraft::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(
		xOffset * mSprite.getGlobalBounds().width, 
		yOffset * mSprite.getGlobalBounds().height);

	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	//Bestimmung der Flugrichtung
	float sign = isAllied() ? -1.f : +1.f;

	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}

/*********************************************************************************
* Erhöt die Anzahl der Raketen um missleAmmo
*
*********************************************************************************/
void Aircraft::collectMissiles(size_t missleAmmo)
{
	mMissileAmmo += missleAmmo;
}

/*********************************************************************************
* Erhöt das Spreadlevel um 1
*
*********************************************************************************/
void Aircraft::increasSpread()
{
	if (mSpreadLevel < 3)
	mSpreadLevel++;
}

/*********************************************************************************
* Erhöt die FireRate um 1
*
*********************************************************************************/
void Aircraft::increasFireRate()
{
		mFireRateLevel++;
}

/*********************************************************************************
* Information über den kleinst möglichen Rahmen um ein Objekt in der Welt
*
*********************************************************************************/
sf::FloatRect Aircraft::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Aircraft::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

void Aircraft::checkPickupCommand(CommandQueue& command)
{
	if (!isAllied() && randomInt(3) == 0)
		command.push(mPickupCommand);
}

void Aircraft::createPickup(SceneNode& node, const TextureHolder& texture) const
{
	auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

	std::unique_ptr<Pickup> pickup(new Pickup(type , texture));

	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 0.1f);
	node.attachChild(std::move(pickup));
}