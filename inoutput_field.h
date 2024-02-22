#include <functional>

#include "SFML/Graphics.hpp"

#include "rounded_rectangle.h"
#include "general_data.h"
#include "window.h"

template<class BackgroundShape>
class InputField : public AbstractElement {
public:
    InputField(sf::Vector2f pos, sf::Vector2f size, const sf::RenderTarget &target);

    void ProcessEvent(sf::Event event, const sf::RenderTarget &target) override;

    void SetPosition(sf::Vector2f pos, const sf::RenderTarget &target) {
        pos_ = {pos.x / target.getSize().x, pos.y / target.getSize().y};
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void Write(sf::Uint32 event);

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
InputField<BackgroundShape>::InputField(sf::Vector2f pos, sf::Vector2f size, const sf::RenderTarget &target)
        : pos_(pos.x / target.getSize().x, pos.y / target.getSize().y)
        , size_(size.x / target.getSize().x, size.y / target.getSize().y)
{}


template<class BackgroundShape>
void InputField<BackgroundShape>::ProcessEvent(sf::Event event, const sf::RenderTarget &target) {
    sf::Vector2f pos(event.touch.x / target.getSize().x, event.touch.y / target.getSize().y);
    active_ = std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 &&
              std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2;
}

template<class BackgroundShape>
void InputField<BackgroundShape>::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    BackgroundShape rect(sf::Vector2f(size_.x * target.getSize().x, size_.y * target.getSize().y));
    rect.setPosition(sf::Vector2f(pos_.x * target.getSize().x, pos_.y * target.getSize().y));
    rect.setOutlineColor(outline_color);
//    rect.setRoundRadius(10.f);
    rect.setFillColor(fill_color);
    rect.setOutlineThickness(2);
    target.draw(rect);

    sf::Text text;
    text.setFont(font);
    text.setString(data_);
    text.setCharacterSize(letter_size);
    text.setPosition(sf::Vector2f(pos_.x * target.getSize().x, pos_.y * target.getSize().y + size_.y * target.getSize().y / 2.f - letter_size * 0.75f));
    text.setFillColor(text_color);
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
                std::cout << event << std::endl;
                data_.push_back(static_cast<char>(event));
        }
    }
}