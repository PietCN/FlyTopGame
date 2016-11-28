#include <TitleState.hpp>
#include <ResourceHolder.hpp>

/*********************************************************************************
* Cstr
* Initialisierung und Zuweisung des Stacks / Context
* Hintergrund wird geladen
* Text wird geladen und positioniert
*
*********************************************************************************/
TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	mText.setFont(context.fonts->get(Fonts::Sansation));
	mText.setString("Weiter mit belieger Taste!");

	mText.setOrigin(mText.getLocalBounds().width / 2.f, mText.getLocalBounds().height / 2.f);
	mText.setPosition(context.window->getView().getSize() / 2.0f);
}

/*********************************************************************************
* HANDLEEVENT
* Fortführung im Programmlauf
* Titel -> Menu
*
*********************************************************************************/
bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}
	
	return true;
}

/*********************************************************************************
* DRAW
*
*********************************************************************************/
void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	if (mShowText)
		window.draw(mText);
}

/*********************************************************************************
* UPDATE
* Steuerung des Textfeldes 'Weiter mit beliebiger Taste!'
*
*********************************************************************************/
bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;

	if (mTextEffectTime > sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}