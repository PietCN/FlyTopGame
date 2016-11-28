#include <Pickup.hpp>
#include <DataTables.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>

namespace {
	const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& texture)
	: Entity(1)
	, mType(type)
	, mSprite(texture.get(Table[type].texture), Table[type].textureRect)
{
	centerOrigin(mSprite);
}

size_t Pickup::getCategory() const
{
	return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Aircraft& player) const
{
	Table[mType].action(player);
}


void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}