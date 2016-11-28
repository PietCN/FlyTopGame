#include <Application.hpp>
#include <TitleState.hpp>
#include <PauseState.hpp>
#include <GameState.hpp>
#include <MenuState.hpp>
#include <SettingState.hpp>
#include <GameOverState.hpp>
#include <State.hpp>
#include <StateIdentifiers.hpp>

const sf::Time Application::TimePerFrame(sf::seconds(1.f / 60.f));

/*********************************************************************************
* Cstr
* Globale Fonts laden
* Starten des Framezählers
* States registrieren
* Start-State initialisieren
*
*********************************************************************************/
Application::Application()
	: mWindow(sf::VideoMode(1024, 800), "Game")
	, mTextures()
	, mFonts()
	, mPlayer()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrame(0)
{
	//Resourcen für das komplette Programm laden
	//werden in der Laufzeit niemals frei gegeben
	mFonts.load(Fonts::Sansation, "Media//Sansation.ttf");
	mTextures.load(Textures::TitleScreen, "Media//Textures//TitleScreen.png");
	mTextures.load(Textures::Buttons, "Media//Textures//Buttons.png");

	mStatisticsText.setFont(mFonts.get(Fonts::Sansation));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Title);
}

/*********************************************************************************
* Start-Methode
* Berechnung der Frametime und fixen Timestep setzen
* Steuerung des Programmablaufs
*
*********************************************************************************/
void Application::run()
{
	sf::Clock clock;
	sf::Time timeSiceLastUpdate = sf::Time::Zero;
	//main-loop
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();

		timeSiceLastUpdate += elapsedTime;
		while (timeSiceLastUpdate > TimePerFrame)
		{
			timeSiceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (mStateStack.isEmpty())
				mWindow.close();
		}
		updateStatistics(elapsedTime);
		render();
	}
}

/*********************************************************************************
* EVENT
* Ausführung der Events im Stack
*
*********************************************************************************/
void Application::processInput()
{
	sf::Event event;

	//event-loop
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

/*********************************************************************************
* UPDATE
* Ausführung der Updates im Stack
*
*********************************************************************************/
void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

/*********************************************************************************
* RENDER
* 
*********************************************************************************/
void Application::render()
{
	mWindow.clear();
	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

/*********************************************************************************
* Berechnung der Frametime
*
*********************************************************************************/
void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrame++;

	if (mStatisticsUpdateTime >= sf::seconds(1.f))
	{
		mStatisticsText.setString(
			"Frames / Seconds : " + std::to_string(mStatisticsNumFrame) + "\n" +
			"Time / Update : " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrame) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.f);
		mStatisticsNumFrame = 0;
	}
}

/*********************************************************************************
* States Registrieren
* ID's werden im StateIdentifier definiert
* zur jeden Registrierung muss ein Datentyp zum entsprechendem State existieren
*
*********************************************************************************/
void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<SettingState>(States::Setting);
	mStateStack.registerState<GameOverState>(States::GameOver);
}