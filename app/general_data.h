#pragma once

#include "SFML/Graphics.hpp"

sf::Font LoadFont(std::string path);

extern sf::Color background_color;
extern sf::Color primary_color;
extern sf::Color outline_color;
extern sf::Color on_primary_color;
extern sf::Color primary_container_color;
extern sf::Color on_primary_container_color;
extern sf::Color surface_container;
extern sf::Color on_surface_container;

extern sf::Font font;
extern float letter_size;
extern sf::Vector2f win_size;
extern sf::Uint32 lambda;
extern long animation_time;
