#pragma once
#ifndef STATE_GUARD
#define STATE_GUARD

#include <memory>
#include <SFML\Graphics.hpp>
#include <StateIdentifiers.hpp>
#include <ResourceIdentifier.hpp>

class StateStack;
class Player;

class State
{
public:
	using Ptr = std::unique_ptr<State>;
	struct Context 
	{
		//Context beinhaltet einen Verweis auf Resourcen und Controller, 
		//die übergreifend von allen Zuständen benutzt werden können.
		Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,	Player& player);

		sf::RenderWindow* window;			//Fenster
		TextureHolder*		textures;		//Texturen	
		FontHolder*				fonts;			//Fonts
		Player*						player;			//Controller
	};

public:
	State::State(StateStack& stack, Context context);

	virtual			State::~State();

	virtual void			draw() = 0;
	virtual bool			update(sf::Time dt) = 0;
	virtual bool			handleEvent(const sf::Event& event) = 0;

protected:
	void							requestStackPush(States::ID stateID);
	void							requestStackPop();
	void							requestStackClear();

	Context						getContext() const;

private:
	StateStack*				mStack;
	Context						mContext;
};

#endif STATE_GUARD