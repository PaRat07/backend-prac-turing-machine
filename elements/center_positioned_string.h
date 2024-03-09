#pragma once

#include "SFML/Graphics.hpp"

#include "../app/general_data.h"

class CenterPositionedString : public sf::Drawable {
 public:
    void setPosition(float x, float y);

    void setPosition(sf::Vector2f pos);

    void setString(sf::String s);

    void setBackground(sf::Color color);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setTextColor(sf::Color color) {
        text_color_ = color;
    }
 private:
    sf::Color text_color_ = on_primary_container_color;
    sf::String data_;
    sf::Vector2f pos_ = { 0, 0 };
};
