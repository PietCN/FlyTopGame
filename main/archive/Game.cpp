#include <Game.hpp>

/*********************************************************************************
* //Test -> kennzeichnet zu Debug zwecken Teststellen um im Entwicklungsprozess 
* die Funktionen zu testen
*********************************************************************************/
/*********************************************************************************
* Game-Class	Static-Member
*********************************************************************************/
const sf::Time Game::TimePerFrame(sf::seconds(1. / 60.));
const float		 Game::PlayerSpeed(100.f);						//TEST

/*********************************************************************************
* Cstr Game-Framework
* 
*********************************************************************************/
Game::Game()
	: mWindow(sf::VideoMode(800, 600), "SFML-WORKS")
	, mIsMovingUp(false)															//TEST
	, mIsMovingDown(false)														//TEST
	, mIsMovingLeft(false)														//TEST
	, mIsMovingRight(false)														//TEST
	, mTextures()																			//TEST
	, mRoot()																					//TEST
	, mPlayerAircraft(nullptr)												//TEST

	, mStatisticsFont()
	, mStatisticsText()
	, mStatisticsUpdateTime(sf::Time::Zero)
	, mStatisticsTimeNumFrame(0)
{
	//Texturen laden
	mTextures.load(Textures::Eagle, "Media//Textures//Eagle.png");
	mTextures.load(Textures::Raptor, "Media//Textures//Raptor.png");
	mTextures.load(Textures::Desert, "Media//Textures//Desert.png");

	//Statistiken initialisieren
	mStatisticsFont.load(Fonts::Sansation, "Media//Sansation.ttf");
	mStatisticsText.setFont(mStatisticsFont.get(Fonts::Sansation));
	mStatisticsText.setPosition(10.f, 10.f);
	mStatisticsText.setCharacterSize(10);

	//SceneKnoten zum Test ein Kind hinzufügen mit Textur und weitere optionen
	std::unique_ptr<SpriteNode> bg(new SpriteNode(mTextures.get(Textures::Desert)));
	mBackground = bg.get();
	mRoot.attachChild(std::move(bg));

	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mTextures));
	mPlayerAircraft = player.get();
	mRoot.attachChild(std::move(player));
	mPlayerAircraft->setPosition(300.f, 200.f);

	std::unique_ptr<Aircraft> rightFollower(new Aircraft(Aircraft::Eagle, mTextures));
	rightFollower->setPosition(80.f, 100.f);
	mPlayerAircraft->attachChild(std::move(rightFollower));

	mPlayerAircraft->setVelocity(50.f, 50.f);

	std::cout << "World-Position.x : " << mRoot.getWorldPosition().x << std::endl;
	std::cout << "World-Position.y : " << mRoot.getWorldPosition().y << std::endl;

	std::cout << "Aircraft World-Position.x : " << mPlayerAircraft->getWorldPosition().x << std::endl;
	std::cout << "Aircraft World-Position.y : " << mPlayerAircraft->getWorldPosition().y << std::endl;
}

/*********************************************************************************
* public Main-Methode
* Startet das Framework von main.cpp aus
*********************************************************************************/
void Game::run()
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
			processEvents();
			update(TimePerFrame);
		}		
		updateStatistics(elapsedTime);
		render();
	}
}

/*********************************************************************************
* UPDATE
*
*********************************************************************************/
void Game::update(sf::Time deltaTime)
{
	//sf::Vector2f movement(0.f, 0.f);

	////example key using W,A,S,D
	//if (mIsMovingUp)
	//	movement.y = -PlayerSpeed;
	//if (mIsMovingDown)
	//	movement.y = +PlayerSpeed;
	//if (mIsMovingLeft)
	//	movement.x = -PlayerSpeed;
	//if (mIsMovingRight)
	//	movement.x = +PlayerSpeed;

	////example end
	mPlayerAircraft->update(deltaTime);
}

/*********************************************************************************
* EVENTS
*
*********************************************************************************/
void Game::processEvents()
{
	sf::Event event;

	//event-loop
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			mWindow.close();
			break;
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
		}
	}
}

/*********************************************************************************
* RENDER
*
*********************************************************************************/
void Game::render()
{
	mWindow.clear();
	mWindow.draw(mStatisticsText);
	mWindow.draw(mRoot);
	mWindow.display();
}

/*********************************************************************************
* Übergibt die betätigten Tasten an das Objekt
*
*********************************************************************************/
void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	//example using key W,A,S,D
	if (key == sf::Keyboard::Key::W)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::Key::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Key::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::Key::D)
		mIsMovingRight = isPressed;
	//example end
}

/*********************************************************************************
* berechnet die Frametime und gibt diese aus
*
*********************************************************************************/
void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsTimeNumFrame++;

	if (mStatisticsUpdateTime >= sf::seconds(1.f))
	{
		mStatisticsText.setString(
			"Frames / Seconds : " + std::to_string(mStatisticsTimeNumFrame) + "\n" +
			"Time / Update : " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsTimeNumFrame) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.f);
		mStatisticsTimeNumFrame = 0;
	}
}