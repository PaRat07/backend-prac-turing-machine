#include "inoutput_field.h"

InputField::InputField(sf::Vector2f pos, sf::Vector2f size, const sf::RenderTarget &target)
        : pos_(pos.x / target.getSize().x, pos.y / target.getSize().y)
        , size_(size.x / target.getSize().x, size.y / target.getSize().y)
{}


void InputField::ProcessEvent(sf::Event event, const sf::RenderTarget &target) {
    sf::Vector2f pos(event.touch.x / target.getSize().x, event.touch.y / target.getSize().y);
    active_ = std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 &&
              std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2;
}

void InputField::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::RoundedRectangleShape rect(size_);
    rect.setPosition(pos_);
    rect.setOutlineColor(outline_color);
    rect.setRoundRadius(10.f);
    rect.setFillColor(fill_color);
    rect.setOutlineThickness(2);
    target.draw(rect);

    sf::Text text;
    text.setFont(font);
    text.setString(data_);
    text.setCharacterSize(letter_size);
    text.setPosition(sf::Vector2f(pos_.x, pos_.y + size_.y / 2.f - letter_size * 0.75f));
    text.setFillColor(text_color);
    target.draw(text);
}

void InputField::Write(sf::Uint32 event) {
    if (active_) {
        switch (event) {
            // Backspace
            case 8:
                if (!data_.empty()) {
                    data_.pop_back();
                }
                break;
            // Enter
            case 42:
                return;
            default:
                std::cout << event << std::endl;
                data_.push_back(static_cast<char>(event));
        }
    }
}


OutputField::OutputField(sf::Vector2f pos, sf::Vector2f size, const sf::RenderTarget &target)
    : pos_(pos.x / target.getSize().x, pos.y / target.getSize().y)
    , size_(size.x / target.getSize().x, size.y / target.getSize().y)
{}


void OutputField::ProcessEvent(sf::Event event, sf::RenderTarget &target) {
    sf::Vector2f pos(event.touch.x / target.getSize().x, event.touch.y / target.getSize().y);
    active_ = std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 &&
              std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2;
}

void OutputField::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::RoundedRectangleShape rect(size_);
    rect.setPosition(pos_);
    rect.setOutlineColor(outline_color);
    rect.setRoundRadius(10.f);
    rect.setFillColor(fill_color);
    rect.setOutlineThickness(2);
    target.draw(rect);

    sf::Text text;
    text.setFont(font);
    text.setString(data_);
    text.setCharacterSize(letter_size);
    text.setPosition(sf::Vector2f(pos_.x, pos_.y + size_.y / 2.f - letter_size * 0.75f));
    text.setFillColor(text_color);
    target.draw(text);
}

void OutputField::SetText(std::string s) {
    data_ = std::move(s);
}
