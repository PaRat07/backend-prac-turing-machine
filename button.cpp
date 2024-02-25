#include "button.h"

void ButtonWithImage::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::RoundedRectangleShape rect(size_);
    rect.setPosition(pos_);
    rect.setOutlineColor(outline_color);
    rect.setFillColor(fill_color);
    rect.setPosition(pos_);
    rect.setRoundRadius(5.f);
    target.draw(rect);


    sf::Texture texture;
    assert(texture.loadFromFile(path_to_png_));
    sf::Sprite sprite(texture);
    float factor = static_cast<float>(std::min(size_.x / static_cast<float>(texture.getSize().x),
                                               size_.y / static_cast<float>(texture.getSize().y)) - 0.01);
    sprite.setScale(factor, factor);
    sprite.setPosition(pos_.x + size_.x / 2.f - static_cast<float>(texture.getSize().x) / 2.f * factor,
                       pos_.y + size_.y / 2.f - static_cast<float>(texture.getSize().x) / 2.f * factor);
    target.draw(sprite);
}

bool ButtonWithImage::ProcessEvent(sf::Event event) {
    if (event.type == sf::Event::MouseEntered) {
        sf::Vector2f pos(event.touch.x, event.touch.y);
        if (std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 && std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2) {
            callback_();
            return true;
        }
    }
    return false;
}

ButtonWithImage::ButtonWithImage(sf::Vector2f pos, sf::Vector2f sz, std::string path, const std::function<void()> &cb)
    : callback_(cb)
    , size_(sz)
    , pos_(pos)
    , path_to_png_(std::move(path))
{}

void ButtonWithTextRelativePos::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::RoundedRectangleShape rect(size_);
    rect.setPosition(pos_);
    rect.setOutlineColor(outline_color);
    rect.setFillColor(fill_color);
    rect.setPosition(pos_);
    rect.setRoundRadius(5.f);
    target.draw(rect);


    CenterPositionedString str;
    str.setString(text_);
    str.setPosition(pos_);
    target.draw(str);
}

bool ButtonWithTextRelativePos::ProcessEvent(sf::Event event) {
    if (event.type == sf::Event::MouseEntered) {
        sf::Vector2f rel_pos(event.touch.x / win_size.x, event.touch.y / win_size.y);
        if (std::abs(rel_pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 && std::abs(rel_pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2) {
            callback_();
            return true;
        }
    }
    return false;
}

ButtonWithTextRelativePos::ButtonWithTextRelativePos(sf::Vector2f pos, sf::Vector2f sz, std::string text, const std::function<void()> &cb)
        : callback_(cb)
        , size_(sz.x / win_size.x, sz.y / win_size.y)
        , pos_((pos.x - sz.x / 2) / win_size.x, (pos.y - sz.y / 2) / win_size.y)
        , text_(std::move(text))
{}

void ButtonWithTextAbsPos::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::RoundedRectangleShape rect(size_);
    rect.setPosition(pos_);
    rect.setOutlineColor(outline_color);
    rect.setFillColor(fill_color);
    rect.setPosition(pos_);
    rect.setRoundRadius(5.f);
    target.draw(rect);


    CenterPositionedString str;
    str.setString(text_);
    str.setPosition(pos_);
    target.draw(str);
}

bool ButtonWithTextAbsPos::ProcessEvent(sf::Event event) {
    if (event.type == sf::Event::MouseEntered) {
        sf::Vector2f pos(event.touch.x, event.touch.y);
        if (std::abs(pos.x - (pos_.x + size_.x / 2)) <= size_.x / 2 && std::abs(pos.y - (pos_.y + size_.y / 2)) <= size_.y / 2) {
            callback_();
            return true;
        }
    }
    return false;
}

ButtonWithTextAbsPos::ButtonWithTextAbsPos(sf::Vector2f pos, sf::Vector2f sz, std::string text, const std::function<void()> &cb)
        : callback_(cb)
        , size_(sz)
        , pos_(pos.x - sz.x / 2, pos.y - sz.y / 2)
        , text_(std::move(text))
{}
