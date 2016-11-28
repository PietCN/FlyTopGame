#pragma once
#ifndef SPRITENODE_GUARD
#define SPRITENODE_GUARD

#include <SceneNode.hpp>

class SpriteNode : public SceneNode
{
public:
	explicit  SpriteNode::SpriteNode(const sf::Texture& texture);
						SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect texturRect);

private:
	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite			mSprite;
};

#endif SPRITENODE_GUARD