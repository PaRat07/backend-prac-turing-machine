#pragma once

#include "SFML/Graphics.hpp"

sf::Font LoadFont(std::string path);

extern sf::Color background_color;
extern sf::Color text_color;
extern sf::Color outline_color;
extern sf::Color fill_color;
extern sf::Font font;
extern float letter_size;
extern sf::Vector2f win_size;
extern sf::String lambda;
