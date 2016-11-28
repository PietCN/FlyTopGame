#pragma once
#ifndef BUTTON_GUARD
#define BUTTON_GUARD

#include <Component.hpp>
#include <memory>
#include <functional>
#include <SFML\Graphics.hpp>

#include <ResourceIdentifier.hpp>
#include <ResourceHolder.hpp>

namespace GUI
{
	class Button : public Component
	{
	public:
		enum ButtonType
		{
			Normal,
			Select,
			Pressed
		};

		using Ptr = std::shared_ptr<Button>;
		using Callback = std::function<void()>;

	public:
		Button::Button(const FontHolder& fonts, const TextureHolder& textures);

		void				setCallback(Callback callback);
		void				setText(const std::string& text);
		void				setToogle(bool flag);

		virtual bool				isSelectable() const;
		virtual void				select();
		virtual void				deselect();

		virtual	void				activate();
		virtual void				deactivate();

		virtual void				handleEvent(const sf::Event& event);

	private:
		virtual void				draw(sf::RenderTarget& target, sf::RenderStates states) const;
						void				centerButtonText();
						void				changeTexture(ButtonType type);
	
	private:
		Callback						mCallback;
		sf::Sprite					mSprite;
		sf::Text						mText;
		bool								mIsToogle;
	};
}


#endif BUTTON_GUARD