#pragma once
#include <TextNode.hpp>
#include <ResourceHolder.hpp>

TextNode::TextNode(const FontHolder& fonts, const std::string& text)
	: mText(text, fonts.get(Fonts::Sansation))
{
	mText.setCharacterSize(20);
}

void TextNode::setString(const std::string& text)
{
	mText.setString(text);
	mText.setOrigin(mText.getLocalBounds().width / 2.f, mText.getLocalBounds().height / 2.f);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText, states);
}