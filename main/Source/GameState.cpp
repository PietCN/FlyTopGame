#include <GameState.hpp>


/*********************************************************************************
* Cstr
* Initialisierung und Zuweisung des Players und Windows zur Game-Struktur
*
*********************************************************************************/
GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(*context.window, *context.fonts)
	, mPlayer(*context.player)
{
	
}

/*********************************************************************************
* DRAW
*
*********************************************************************************/
void GameState::draw()
{
	mWorld.draw();
}

/*********************************************************************************
* UPDATE
* Auffangen der RealTime-Inputs
*
*********************************************************************************/
bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	if (!mWorld.hasAlivePlayer())
	{
		mPlayer.setMissionStatus(Player::MissionFailure);
		requestStackPush(States::GameOver);
	}
	else if (mWorld.hasPlayerReachedEnd())
	{
		mPlayer.setMissionStatus(Player::MissionSuccess);
		requestStackPush(States::GameOver);
	}

	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealTimeInput(commands);

	return true;
}

/*********************************************************************************
* EVENT
* Auffangen der Event-Inputs
*
*********************************************************************************/
bool GameState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = mWorld.getCommandQueue();

	mPlayer.handleEvent(event, commands);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}