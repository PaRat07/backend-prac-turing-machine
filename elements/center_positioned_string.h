#pragma once

#include "SFML/Graphics.hpp"

#include "../app/general_data.h"

class CenterPositionedString : public sf::Drawable {
 public:
    void setPosition(float x, float y);

    void setPosition(sf::Vector2f pos);

    void setString(std::string s);

    void setBackground(sf::Color color);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
 private:
    sf::Color background_color_ = text_color;
    std::string data_;
    sf::Vector2f pos_ = { 0, 0 };
};
