#pragma once

#include <functional>

#include "SFML/Graphics.hpp"

#include "rounded_rectangle.h"
#include "../app/general_data.h"
#include "../app/window.h"
#include "center_positioned_string.h"

template<class BackgroundShape>
class InputField : public AbstractElement {
public:
    InputField(sf::Vector2f pos, sf::Vector2f size);

    void ProcessEvent(sf::Event event) override;

    void SetPosition(sf::Vector2f pos) {
        pos_ = {pos.x / win_size.x, pos.y / win_size.y};
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void Write(sf::Uint32 event);

    void Activate() {
        active_ = true;
    }

    const std::string &GetText() const {
        return data_;
    }

private:
    sf::Vector2f pos_, size_;
    std::string data_;
    bool active_ = false;
};

class OutputField : public AbstractElement {
public:
    OutputField(sf::Vector2f pos, sf::Vector2f size, const sf::RenderTarget &target);

    void ProcessEvent(sf::Event event, sf::RenderTarget &target);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void SetText(std::string s);

private:
    sf::Vector2f pos_, size_;
    std::string data_;
    bool active_ = false;
};

template<class BackgroundShape>
InputField<BackgroundShape>::InputField(sf::Vector2f pos, sf::Vector2f size)
        : pos_(pos.x / win_size.x, pos.y / win_size.y)
        , size_(size.x / win_size.x, size.y / win_size.y)
{
}


template<class BackgroundShape>
void InputField<BackgroundShape>::ProcessEvent(sf::Event event) {
    switch (event.type) {
        case sf::Event::MouseButtonPressed: {
            sf::Vector2f pos(event.touch.x / win_size.x, event.touch.y / win_size.y);
            active_ = std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 &&
                      std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2;
            break;
        }
        case sf::Event::TextEntered:
            Write(event.text.unicode);
            break;
    }
}

template<class BackgroundShape>
void InputField<BackgroundShape>::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    BackgroundShape rect(sf::Vector2f(size_.x * win_size.x, size_.y * win_size.y));
    rect.setPosition(sf::Vector2f(pos_.x * win_size.x, pos_.y * win_size.y));
    rect.setOutlineColor(outline_color);
//    rect.setRoundRadius(10.f);
    rect.setFillColor(fill_color);
    rect.setOutlineThickness(2);
    target.draw(rect);

    CenterPositionedString text;
    text.setString(data_);
    text.setPosition(sf::Vector2f(pos_.x * win_size.x + size_.x * win_size.x / 2, pos_.y * win_size.y + size_.y * win_size.y - letter_size * 0.75f));
    target.draw(text);
}

template<class BackgroundShape>
void InputField<BackgroundShape>::Write(sf::Uint32 event) {
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
                data_.push_back(static_cast<char>(event));
        }
    }
}