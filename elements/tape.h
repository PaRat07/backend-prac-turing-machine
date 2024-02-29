#pragma once

#include <chrono>
#include <optional>

#include "SFML/Graphics.hpp"

#include "../app/window.h"
#include "../app/general_data.h"
#include "../turing_machine.h"
#include "center_positioned_string.h"
#include "inoutput_field.h"

class Tape : public AbstractElement {
    struct Animation {
        float from_pos, to_pos;
        std::chrono::time_point<std::chrono::steady_clock> from_time, to_time;
    };

public:
    static sf::Vector2i cell_size;

    Tape(TuringMachine &mach, float y_pos);

    void ProcessEvent(sf::Event event);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void MoveRight() {
        if (!animation_.has_value()) {
            animation_ ={pos_in_, pos_in_ + cell_size.x,
                               std::chrono::steady_clock::now(),
                               std::chrono::steady_clock::now() + std::chrono::milliseconds(animation_time)
                        };
        }
    }

    void MoveLeft() {
        if (!animation_.has_value()) {
            animation_ ={pos_in_, pos_in_ - cell_size.x,
                         std::chrono::steady_clock::now(),
                         std::chrono::steady_clock::now() + std::chrono::milliseconds(animation_time)
            };
        }
    }

    // ms
    long animation_time = 1'000;
private:
    TuringMachine &machine_;
    float pos_in_ = 0;
    float y_pos_;
    std::optional<int> active_pos_;
    std::optional<Animation> animation_;
};