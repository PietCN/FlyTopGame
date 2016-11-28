#include <Label.hpp>
#include <ResourceHolder.hpp>

namespace GUI
{
	/*********************************************************************************
	* Cstr
	* Initialisierung des Textes
	*
	*********************************************************************************/
	Label::Label(const std::string& text, const FontHolder& fonts)
		: mText(text, fonts.get(Fonts::Sansation), 16)
	{

	}

	/*********************************************************************************
	* Labels sind nicht anwählbar
	*
	*********************************************************************************/
	bool Label::isSelectable() const
	{
		return false;
	}

	/*********************************************************************************
	* Label vom Render-Ziel zeichnen lassen
	*
	*********************************************************************************/
	void Label::draw(sf::RenderTarget& targer, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		targer.draw(mText, states);
	}

	/*********************************************************************************
	* Text des Labels festlegen
	*
	*********************************************************************************/
	void Label::setText(const std::string& string)
	{
		mText.setString(string);
	}

	/*********************************************************************************
	* Label hat keine interaktion mit dem Benutzer
	*
	*********************************************************************************/
	void Label::handleEvent(const sf::Event& event)
	{

	}
}