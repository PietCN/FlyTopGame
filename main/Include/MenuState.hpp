#pragma once
#ifndef MENUSTATE_GUARD
#define MENUSTATE_GUARD

#include <State.hpp>
#include <Container.hpp>
#include <ResourceIdentifier.hpp>
#include <ResourceHolder.hpp>

class MenuState : public State
{
public:
	MenuState::MenuState(StateStack& stack, Context context);

	virtual void						draw();
	virtual bool						update(sf::Time dt);
	virtual bool						handleEvent(const sf::Event& event);

private:
	void										loadTextures();

private:
	sf::Sprite							mBackgroundSprite;
	GUI::Container					mGUIContainer;
	TextureHolder						mTextures;
};


#endif MENUSTATE_GUARD