#pragma once

#include "SFML/Graphics.hpp"

sf::Font LoadFont(std::string path);

static sf::Color background_color = sf::Color(152, 147, 218);
static sf::Color text_color = sf::Color(0, 0, 0);
static sf::Color outline_color = sf::Color(114, 114, 126);
static sf::Color fill_color = sf::Color(187, 189, 246);
static sf::Font font = LoadFont("../JetBrainsMono-Medium.ttf");
static float letter_size = 20.f;
