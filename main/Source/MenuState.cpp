#include <MenuState.hpp>
#include <ResourceHolder.hpp>
#include <iostream>
#include <Button.hpp>

/*********************************************************************************
* Cstr - Initialisierung
* Texte für Menüpunkte werden geladen und in den Option-Vector geladen
* 
*********************************************************************************/
MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
	, mTextures()
{
	//Texturen laden über context
	loadTextures();
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::Sansation);

	mBackgroundSprite.setTexture(texture);

	//PLAY_BUTTON
	auto playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	playButton->setPosition(100.f, 250.f );
	playButton->setText("Play");
	playButton->setCallback( [this] () 
	{
		requestStackPop();
		requestStackPush(States::Game);
	});

	//SETTINGS_BUTTON
	auto SettingButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	SettingButton->setPosition(100.f, 300.f);
	SettingButton->setText("Setting");
	SettingButton->setCallback([this]()

	{ 
		requestStackPush(States::Setting);
	});

	//EXIT_BUTTON
	auto exitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	exitButton->setPosition(100.f, 350.f);
	exitButton->setText("Exit");
	exitButton->setCallback([this]()

	{ 
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(SettingButton);
	mGUIContainer.pack(exitButton);
	
}
 
/*********************************************************************************
* alle Menüpunkte im Optin-Vector werden gezeichnet
*
*********************************************************************************/
void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
	/*for (auto& options : mOptions)
		window.draw(options);*/

	

}

/*********************************************************************************
* keine Funktion
*
*********************************************************************************/
bool MenuState::update(sf::Time dt)
{

	return true;
}

/*********************************************************************************
* Menüauswahl steuerung über Index
*
*********************************************************************************/
bool MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}

void MenuState::loadTextures()
{
	mTextures.load(Textures::Buttons, "Media//Textures//Buttons.png");
}