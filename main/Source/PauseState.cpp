#include <PauseState.hpp>
#include <ResourceHolder.hpp>

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
{
	sf::Font& font = context.fonts->get(Fonts::Sansation);
	sf::Vector2f viewSize = context.window->getView().getSize();

	mPauseText.setFont(font);
	mPauseText.setString("Game Paused");
	mPauseText.setCharacterSize(70);
	mPauseText.setOrigin(mPauseText.getLocalBounds().width / 2.f, mPauseText.getLocalBounds().height / 2.f);
	mPauseText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

	mInstructionText.setFont(font);
	mInstructionText.setString("(Drücke Backspace um ins Menü zurück zu kommen)");
	mInstructionText.setCharacterSize(15);
	mInstructionText.setOrigin(mInstructionText.getLocalBounds().width / 2.f, mInstructionText.getLocalBounds().height / 2.f);
	mInstructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
	
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(sf::Vector2f(window.getSize()));

	window.draw(backgroundShape);
	window.draw(mPauseText);
	window.draw(mInstructionText);
}
bool PauseState::update(sf::Time dt)
{

	return false;
}
bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == event.KeyPressed)
		switch (event.key.code)
		{
		case sf::Keyboard::Escape :
			requestStackPop();
			break;
		case sf::Keyboard::BackSpace :
			requestStackClear();
			requestStackPush(States::Menu);
		}

	return false;
}