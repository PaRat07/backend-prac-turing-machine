#include "../elements/tape.h"


sf::Vector2i Tape::cell_size = { 50, 50 };

Tape::Tape(TuringMachine &mach, float y_pos)
        : machine_(mach)
        , y_pos_(y_pos)
{}

void Tape::ProcessEvent(sf::Event event) {
    switch (event.type) {
        case sf::Event::MouseButtonPressed: {
            if (std::abs(y_pos_ + cell_size.y / 2 - event.mouseButton.y) < cell_size.y / 2) {
                active_pos_ = (event.mouseButton.x + pos_in_) / cell_size.x;
            } else {
                active_pos_.reset();
            }
            break;
        }
        case sf::Event::TextEntered: {
            if (active_pos_.has_value()) {
                machine_.Write(*active_pos_, event.text.unicode);
                ++*active_pos_;
            }
            break;
        }
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::Wheel::HorizontalWheel) {
                pos_in_ -= event.mouseWheelScroll.delta;
            }
            break;
    }
}

void Tape::draw(sf::RenderTarget &target, sf::RenderStates states) const {
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
        for (int x = -(int)pos_in_ % cell_size.x; x < win_size.x; x += cell_size.x) {
            line.setPosition(x, y_pos_);
            target.draw(line);
        }
    }

    // vals
    {
        CenterPositionedString str;
        for (int i = -pos_in_ / cell_size.x - 1; (i - 1)  * cell_size.x - pos_in_ <= win_size.x; ++i) {
            str.setString(std::string(1, machine_.Read(i)));
            str.setPosition(i * cell_size.x - pos_in_ - cell_size.x / 2, y_pos_ + cell_size.y / 2);
            target.draw(str);
        }
    }

    // active
    if (active_pos_.has_value()) {
        sf::RectangleShape rect((sf::Vector2f(cell_size)));
        rect.setPosition(*active_pos_ * cell_size.x + pos_in_, y_pos_);
    }
}
