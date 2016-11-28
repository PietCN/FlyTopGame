#include <Projectile.hpp>
#include <EmitterNode.hpp>

#include <ResourceHolder.hpp>
#include <DataTables.hpp>
#include <Utility.hpp>

#include <cassert>

namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

/*********************************************************************************
* Cstr
*
*********************************************************************************/
Projectile::Projectile(Type type, const TextureHolder& textures)
	: Entity(1)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mTargetDirection()
{
	centerOrigin(mSprite);

	//Partikelsystem hinzufügen
	if (isGuided())
	{
		std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke));
		smoke->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(smoke));

		std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant));
		propellant->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(propellant));
	}
}

/*********************************************************************************
* mTargetDirection erhält einen Vector mit der Länge 1 der auf die momentane
* Position des Ziels zeigt
*
*********************************************************************************/
void Projectile::guideTowards(sf::Vector2f position)
{
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());
}

/*********************************************************************************
* Cstr
*
*********************************************************************************/
bool Projectile::isGuided() const
{
	return mType == Missile;
}

/*********************************************************************************
* Cstr
*
*********************************************************************************/
size_t Projectile::getCategory() const
{
	if (mType == EnemyBullet)
		return Category::EnemyProjectile;
	else
		return Category::AlliedProjectile;
}

/*********************************************************************************
* Cstr
*
*********************************************************************************/
sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

/*********************************************************************************
* Cstr
*
*********************************************************************************/
float Projectile::getMaxSpeed() const
{
	return Table[mType].speed;
}

/*********************************************************************************
* Cstr
*
*********************************************************************************/
int Projectile::getDamage() const
{
	return Table[mType].damage;
}

/*********************************************************************************
* Update
*
*********************************************************************************/
void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isGuided())
	{
		const float approachRate = 200.f;
		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());

		newVelocity *= getMaxSpeed();
		float angle = std::atan2(newVelocity.y, newVelocity.x);

		setRotation(toDegree(angle) + 90.f);
		setVelocity(newVelocity);
	}

	Entity::updateCurrent(dt, commands);
}

/*********************************************************************************
* Draw
*
*********************************************************************************/
 void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
 {
	 target.draw(mSprite, states);
 }
