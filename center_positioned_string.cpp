#include "center_positioned_string.h"

void CenterPositionedString::setPosition(float x, float y) {
    pos_ = { x, y };
}

void CenterPositionedString::setPosition(sf::Vector2f pos) {
    pos_ = pos;
}

void CenterPositionedString::setString(std::string s) {
    data_ = std::move(s);
}

void CenterPositionedString::setBackground(sf::Color color) {
    background_color_ = color;
}

void CenterPositionedString::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Text val;
    val.setFont(font);
    val.setFillColor(background_color_);
    val.setCharacterSize(letter_size);
    val.setString(data_);
    val.setPosition(pos_.x - val.getLocalBounds().width / 2, pos_.y - letter_size / 1.75f);
    target.draw(val);
}
