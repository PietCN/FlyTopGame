#include <Button.hpp>
#include <Utility.hpp>
#include <iostream>

namespace GUI
{

	/*********************************************************************************
	* Cstr
	* Textur wird zugeordnet und der Text zentriert
	*
	*********************************************************************************/
	Button::Button(const FontHolder& fonts, const TextureHolder& textures)
		: mCallback()
		, mSprite(textures.get(Textures::Buttons))
		, mText("", fonts.get(Fonts::Sansation), 16)
		, mIsToogle(false)
	{
		changeTexture(Normal);

		centerButtonText();
	}

	/*********************************************************************************
	* Fügt eine Aktion zum Button hinzu
	*
	*********************************************************************************/
	void Button::setCallback(Callback callback)
	{
		mCallback = std::move(callback);
	}

	/*********************************************************************************
	* Wechsel der Texture auf angegebenen Typ
	*
	*********************************************************************************/
	void Button::changeTexture(ButtonType type)
	{
		sf::IntRect textureRect(0, 50 * type, 200, 50);
		mSprite.setTextureRect(textureRect);

	}

	/*********************************************************************************
	* Setzt einen Text auf den Button
	*
	*********************************************************************************/
	void Button::setText(const std::string& text)
	{
		sf::FloatRect bounds = mSprite.getLocalBounds();

		mText.setString(text);
		centerButtonText();
	}

	/*********************************************************************************
	* Steuert das Toogleflag
	* toggle == false : Button wirkt als Taster
	* toggle == true  : Button wirkt als Schalter
	*
	*********************************************************************************/
	void Button::setToogle(bool flag)
	{
		mIsToogle = flag;
	}

	/*********************************************************************************
	* Ein Button ist Selektierbar
	*
	*********************************************************************************/
	bool Button::isSelectable() const
	{
		return true;
	}

	/*********************************************************************************
	* Selektiert einen Button
	*
	*********************************************************************************/
	void Button::select()
	{
		Component::select();
		changeTexture(Select);
	}

	/*********************************************************************************
	* Deselektiert einen Button
	*
	*********************************************************************************/
	void Button::deselect()
	{
		Component::deselect();
		changeTexture(Normal);
	}

	/*********************************************************************************
	* Aktiviert einen selektierten Button und fürt die Aktion durch
	* ist Toggle true, wird der Button nicht automatisch wieder deaktiviert
	*
	*********************************************************************************/
	void Button::activate()
	{
		Component::activate();

		if (mIsToogle)
			changeTexture(Pressed);
		if (mCallback)
			mCallback();
		if (!mIsToogle)
			deactivate();
	}

	/*********************************************************************************
	* Deactiviert einen selektierten Button 
	* (mToggle muss dafür auf true gesetzt werden)
	*
	*********************************************************************************/
	void Button::deactivate()
	{
		Component::deactivate();
		if (mIsToogle)
		{
			if (isSelected())
				changeTexture(Select);
			else
				changeTexture(Normal);
		}
	}

	/*********************************************************************************
	* 
	*
	*********************************************************************************/
	void Button::handleEvent(const sf::Event& event)
	{
		//do nothing
	}

	/*********************************************************************************
	* Malt das Objekt auf dem Bildschirm
	*
	*********************************************************************************/
	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		target.draw(mSprite, states);
		target.draw(mText, states);
	}

	/*********************************************************************************
	* Zentralisiert den Text im Button
	*
	* Muss immer eingesetzt werden, wenn der Text geändert wird und sich dieser
	* Zentral im Button befinden soll.
	*
	*********************************************************************************/
	void Button::centerButtonText()
	{
		sf::FloatRect bounds = mSprite.getLocalBounds();

		centerText(mText);
		mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
	}
}

