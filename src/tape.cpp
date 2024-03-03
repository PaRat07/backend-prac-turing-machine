#include "../elements/tape.h"


sf::Vector2i Tape::cell_size = { 100, 50 };

Tape::Tape(TuringMachine &mach, float y_pos)
        : machine_(mach)
        , y_pos_(y_pos)
{}

void Tape::ProcessEvent(sf::Event event) {
    switch (event.type) {
        case sf::Event::MouseButtonPressed: {
            if (std::abs(y_pos_ + cell_size.y / 2 - event.mouseButton.y) < cell_size.y / 2) {
                active_pos_ = (event.mouseButton.x - (pos_in_ + win_size.x / 2) - cell_size.x / 2) / cell_size.x;
            } else {
                active_pos_.reset();
            }
            break;
        }
        case sf::Event::TextEntered: {
            if (active_pos_.has_value()) {
                if (event.text.unicode == 8) {
                    machine_.Write(*active_pos_, lambda);
                    --*active_pos_;
                } else {
                    machine_.Write(*active_pos_, event.text.unicode);
                    ++*active_pos_;
                }
            }
            break;
        }
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::Wheel::HorizontalWheel) {
                pos_in_ += event.mouseWheelScroll.delta * 3;
            }
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Left) {
                MoveLeft();
            } else if (event.key.code == sf::Keyboard::Right) {
                MoveRight();
            }
            break;
    }
}

void Tape::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (animation_.has_value()) {
        auto cur_time = std::chrono::steady_clock::now();
        if (animation_->to_time <= cur_time) {
            pos_in_ = animation_->to_pos;
            animation_.reset();
        } else {
            pos_in_ = animation_->from_pos + (animation_->to_pos - animation_->from_pos) * CalcPercentage(1. * (cur_time - animation_->from_time).count() / (animation_->to_time - animation_->from_time).count());
        }
    }
    // up+down
    {
        sf::RectangleShape line;
        line.setFillColor(outline_color);
        line.setSize(sf::Vector2f(win_size.x, 2));

        line.setPosition(0, y_pos_);
        target.draw(line);

        line.setPosition(0, y_pos_ + cell_size.y);
        target.draw(line);
    }

    // sep
    {
        sf::RectangleShape line;
        line.setFillColor(outline_color);

        line.setSize(sf::Vector2f(2, cell_size.y));
        for (int x = (int)(pos_in_ + win_size.x / 2) % cell_size.x + cell_size.x / 2; x < win_size.x; x += cell_size.x) {
            line.setPosition(x, y_pos_);
            target.draw(line);
        }
    }

    // vals
    {
        CenterPositionedString str;
        for (int i = (-pos_in_ - win_size.x / 2) / cell_size.x - 1; (i - 1)  * cell_size.x + (pos_in_ - win_size.x / 2) <= win_size.x; ++i) {
            str.setString(sf::String(machine_.Read(i)));
            str.setPosition(i * cell_size.x + (pos_in_ + win_size.x / 2), y_pos_ + cell_size.y / 2);
            target.draw(str);
        }
    }

    // active
    if (active_pos_.has_value()) {
        sf::RectangleShape rect((sf::Vector2f(cell_size.x - 2, cell_size.y - 2)));
        rect.setFillColor(fill_color);
        rect.setPosition(*active_pos_ * cell_size.x + (pos_in_ + win_size.x / 2) - cell_size.x / 2 + 2, y_pos_ + 2);
        target.draw(rect);
    }
}


double Tape::CalcPosTapeHead(double time) {
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

double Tape::CalcPercentage(double time) {
    time *= 4.;
    if (time < 1.) {
        return time * time / 6. - CalcPosTapeHead(time / 4) * 0.5;
    }
    if (time < 3.) {
        return (time - 0.5) / 3. - CalcPosTapeHead(time / 4) * 0.5;
    }
    return 1. - std::pow(4. - time, 2.) / 6. - CalcPosTapeHead(time / 4) * 0.5;
}

void Tape::MoveRight() {
    if (!animation_.has_value()) {
        animation_ ={pos_in_, pos_in_ - cell_size.x,
                     std::chrono::steady_clock::now(),
                     std::chrono::steady_clock::now() + std::chrono::milliseconds(animation_time)
        };
    }
}

void Tape::MoveLeft() {
    if (!animation_.has_value()) {
        animation_ ={pos_in_, pos_in_ + cell_size.x,
                     std::chrono::steady_clock::now(),
                     std::chrono::steady_clock::now() + std::chrono::milliseconds(animation_time)
        };
    }
}
