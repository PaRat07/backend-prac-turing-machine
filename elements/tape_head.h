#include <chrono>
#include <optional>
#include <cmath>

#include "../app/window.h"
#include "../app/general_data.h"

class TapeHead : public AbstractElement {
    struct Animation {
        int direction;
        std::chrono::time_point<std::chrono::steady_clock> beg_time, end_time;
    };

public:
    TapeHead(float y_pos)
        : y_pos_(y_pos)
    {}

    void ProcessEvent(sf::Event event) override {
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
    void MoveRight() {
        if (!animation_.has_value()) {
            animation_ = {-1,
                         std::chrono::steady_clock::now(),
                         std::chrono::steady_clock::now() + std::chrono::milliseconds(animation_time)
            };
        }
    }

    void MoveLeft() {
        if (!animation_.has_value()) {
            animation_ = {1,
                          std::chrono::steady_clock::now(),
                          std::chrono::steady_clock::now() + std::chrono::milliseconds(animation_time)
            };
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
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

    static float move_for;
    static long animation_time;
private:
    float y_pos_;
    mutable std::optional<Animation> animation_;


    static double CalcPos(double time) {
        return std::sin(time * M_PI);
    }
};
