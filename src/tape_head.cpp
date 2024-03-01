#include "../elements/tape_head.h"

float TapeHead::move_for = 50.;

TapeHead::TapeHead(float y_pos)
        : y_pos_(y_pos)
{}

void TapeHead::ProcessEvent(sf::Event event) {
    switch (event.type) {
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Left) {
                MoveLeft();
            } else if (event.key.code == sf::Keyboard::Right) {
                MoveRight();
            }
            break;
    }
}

void TapeHead::MoveRight() {
    if (!animation_.has_value()) {
        animation_ = {1,
                      std::chrono::steady_clock::now(),
                      std::chrono::steady_clock::now() + std::chrono::milliseconds(animation_time)
        };
    }
}

void TapeHead::MoveLeft() {
    if (!animation_.has_value()) {
        animation_ = {-1,
                      std::chrono::steady_clock::now(),
                      std::chrono::steady_clock::now() + std::chrono::milliseconds(animation_time)
        };
    }
}

void TapeHead::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::ConvexShape convex;
    convex.setFillColor(sf::Color::Transparent);
    convex.setPointCount(3);
    convex.setPoint(0, sf::Vector2f(win_size.x / 2, y_pos_));
    convex.setPoint(1, sf::Vector2f(win_size.x / 2 + 15, y_pos_ + 30));
    convex.setPoint(2, sf::Vector2f(win_size.x / 2 - 15, y_pos_ + 30));
    convex.setFillColor(sf::Color::Transparent);
    convex.setOutlineThickness(3);
    convex.setOutlineColor(outline_color);
    if (animation_.has_value()) {
        auto cur_time = std::chrono::steady_clock::now();
        if (cur_time >= animation_->end_time) {
            animation_.reset();
        } else {
            double cur_rel_time = 1. * (cur_time - animation_->beg_time).count() / (animation_->end_time - animation_->beg_time).count();
            sf::Vector2f shift(move_for * CalcPos(cur_rel_time) * animation_->direction, 0);
            convex.move(shift);
        }
    }
    target.draw(convex);
}

double TapeHead::CalcPos(double time) {
    time *= 4;
    if (time < 1) {
        return time / 3;
    }
    --time;
    if (time < 2) {
        return 1. / 3;
    }
    time -= 2;
    return (1 - time) / 3;
}
