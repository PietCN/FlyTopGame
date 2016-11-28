#include <SettingState.hpp>

#include <Utility.hpp>
#include <ResourceIdentifier.hpp>
#include <ResourceHolder.hpp>
#include <string>

/*********************************************************************************
* Cstr
* Buttons und zugehörige Labels initialisiert
* Back-Button initialisiert
*
*********************************************************************************/
SettingState::SettingState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	addButtonLabel(Player::MoveLeft,		 150.f, "Move Left",		 context);
	addButtonLabel(Player::MoveRight,		 200.f, "Move Right",		 context);
	addButtonLabel(Player::MoveUp,			 250.f, "Move Up",			 context);
	addButtonLabel(Player::MoveDown,		 300.f, "Move Down",		 context);
	addButtonLabel(Player::Fire,				 350.f, "Fire",					 context);
	addButtonLabel(Player::LaunchMissle, 400.f, "Launch Missle", context);

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backButton->setPosition(400.f, 375.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingState::requestStackPop, this));

	mGUIContainer.pack(backButton);
}

/*********************************************************************************
*
*
*********************************************************************************/
void SettingState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

/*********************************************************************************
*
*
*********************************************************************************/
bool SettingState::update(sf::Time dt)
{
	return true;
}

/*********************************************************************************
* Geht den Container durch und überprüft ob einer aktiv ist
*
*********************************************************************************/
bool SettingState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	for (std::size_t action = 0; action < Player::ActionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyPressed)
			{
				getContext().player->assignKey(static_cast<Player::Action> (action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	if (isKeyBinding)
		updateLabels();
	else
		mGUIContainer.handleEvent(event);

	return false;
}

/*********************************************************************************
* Aktuallisiert die Texte auf den Labels
*
*********************************************************************************/
void SettingState::updateLabels()
{
	Player& player = *getContext().player;

	for (std::size_t i = 0; i < Player::ActionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignKey(static_cast<Player::Action> (i));
		mBindingLabels[i]->setText(toString(key));
	}
}

/*********************************************************************************
* Fügt einen Button und zugehöriges Label dem Container hinzu
*
*********************************************************************************/
void SettingState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context)
{
	mBindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mBindingButtons[action]->setPosition(80.f, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToogle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
	mBindingLabels[action]->setPosition(300.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}