#pragma once

#include <functional>

#include "SFML/Graphics.hpp"

#include "rounded_rectangle.h"
#include "../app/general_data.h"
#include "../app/window.h"
#include "center_positioned_string.h"

template<typename BackgroundShape>
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


template<typename BackgroundShape>
class OutputField : public AbstractElement {
public:
    OutputField(sf::Vector2f pos, sf::Vector2f size);

    void ProcessEvent(sf::Event event) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void SetText(std::string s);

private:
    sf::Vector2f pos_, size_;
    std::string data_;
    bool active_ = false;
};

template<typename BackgroundShape>
InputField<BackgroundShape>::InputField(sf::Vector2f pos, sf::Vector2f size)
        : pos_(pos.x / win_size.x, pos.y / win_size.y)
        , size_(size.x / win_size.x, size.y / win_size.y)
{
}


template<typename BackgroundShape>
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

template<typename BackgroundShape>
void InputField<BackgroundShape>::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Vector2f real_size(size_.x * win_size.x, size_.y * win_size.y), real_pos(pos_.x * win_size.x, pos_.y * win_size.y);
    BackgroundShape rect(real_size);
    rect.setPosition(real_pos);
    rect.setOutlineThickness(0);
    rect.setFillColor(primary);
    target.draw(rect);

    CenterPositionedString text;
    text.setString(data_);
    text.setTextColor(on_primary);
    text.setPosition(sf::Vector2f(real_pos.x + real_size.x / 2, real_pos.y + real_size.y - letter_size * 0.75f));
    target.draw(text);
}

template<typename BackgroundShape>
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

template<typename BackgroundShape>
OutputField<BackgroundShape>::OutputField(sf::Vector2f pos, sf::Vector2f size)
        : pos_(pos.x / win_size.x, pos.y / win_size.y)
        , size_(size.x / win_size.x, size.y / win_size.y)
{}


template<typename BackgroundShape>
void OutputField<BackgroundShape>::ProcessEvent(sf::Event event) {
    sf::Vector2f pos(event.touch.x / win_size.x, event.touch.y / win_size.y);
    active_ = std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 &&
              std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2;
}

template<typename BackgroundShape>
void OutputField<BackgroundShape>::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    BackgroundShape rect(sf::Vector2f(size_.x * win_size.x, size_.y * win_size.y));
    rect.setPosition(sf::Vector2f(pos_.x * win_size.x, pos_.y * win_size.y));
    rect.setOutlineThickness(0);
    rect.setFillColor(primary);
    rect.setOutlineThickness(2);
    target.draw(rect);

    sf::Text text;
    text.setFont(font);
    text.setString(data_);
    text.setCharacterSize(letter_size);
    text.setPosition(sf::Vector2f(pos_.x, pos_.y + size_.y / 2));
    text.setFillColor(on_primary);
    target.draw(text);
}

template<typename BackgroundShape>
void OutputField<BackgroundShape>::SetText(std::string s) {
    data_ = std::move(s);
}
