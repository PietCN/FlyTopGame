#pragma once
#ifndef LABEL_GUARD
#define LABEL_GUARD

#include <Component.hpp>
#include <string>
#include <ResourceIdentifier.hpp>

namespace GUI
{
	class Label : public Component
	{
	public:
		using Ptr = std::shared_ptr<Label>;

							 Label::Label(const std::string& text, const FontHolder& fonts);

		virtual	bool			isSelectable() const;
											
						
						void			setText(const std::string& text);
		virtual void			handleEvent(const sf::Event& event);

	private:
		void			draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Text	mText;
	};
}


#endif LABEL_GUARD