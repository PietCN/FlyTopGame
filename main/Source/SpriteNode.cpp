#include <SpriteNode.hpp>

/*********************************************************************************
* 
*
*********************************************************************************/
SpriteNode::SpriteNode(const sf::Texture& texture)
	: mSprite(texture)
{

}

/*********************************************************************************
* 
*
*********************************************************************************/
SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect texturRect)
	: mSprite(texture, texturRect)
{

}

/*********************************************************************************
* 
*
*********************************************************************************/
void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}