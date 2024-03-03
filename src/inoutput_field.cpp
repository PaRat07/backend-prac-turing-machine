#include "../elements/inoutput_field.h"



OutputField::OutputField(sf::Vector2f pos, sf::Vector2f size)
    : pos_(pos.x / win_size.x, pos.y / win_size.y)
    , size_(size.x / win_size.x, size.y / win_size.y)
{}


void OutputField::ProcessEvent(sf::Event event) {
    sf::Vector2f pos(event.touch.x / win_size.x, event.touch.y / win_size.y);
    active_ = std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 &&
              std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2;
}

void OutputField::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    RoundedRectangleShape<10> rect(sf::Vector2f(size_.x * win_size.x, size_.y * win_size.y));
    rect.setPosition(sf::Vector2f(pos_.x * win_size.x, pos_.y * win_size.y));
    rect.setOutlineColor(outline_color);
    rect.setFillColor(fill_color);
    rect.setOutlineThickness(2);
    target.draw(rect);

    sf::Text text;
    text.setFont(font);
    text.setString(data_);
    text.setCharacterSize(letter_size);
    text.setPosition(sf::Vector2f(pos_.x, pos_.y + size_.y / 2));
    text.setFillColor(text_color);
    target.draw(text);
}

void OutputField::SetText(std::string s) {
    data_ = std::move(s);
}
