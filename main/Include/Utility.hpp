#pragma once
#ifndef UTILITY_GUARD
#define UTILITY_GUARD

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <sstream>

namespace sf
{
	class Sprite;
	class Text;
}

//Umwandeln von einer Enumeration zu einem String
std::string toString(sf::Keyboard::Key key);

//Umwandung einer Zahl zu einem String
template <typename T>
std::string toString(const T& value)
{
	std::stringstream stream;

	stream << value;
	return stream.str();
}

//Orign auf Center des Objektes setzen
void								centerOrigin(sf::Sprite& sprite);
void								centerText(sf::Text& text);

//Winkelberechnungen
float								toDegree(float radian);
float								toRadian(float degree);

//Vector Operationen
float								length(sf::Vector2f vector);
sf::Vector2f				unitVector(sf::Vector2f vector);

//Zufallszahlen
int randomInt(int exclusiceMax);

#endif UTILITY_GUARD