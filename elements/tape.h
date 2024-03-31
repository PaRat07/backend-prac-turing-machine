#pragma once

#include <chrono>
#include <optional>
#include <cmath>

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

    void MoveRight();

    void MoveLeft();

private:
    TuringMachine &machine_;
    mutable float pos_in_ = 0;
    float y_pos_;
    std::optional<int> active_pos_;
    mutable std::optional<Animation> animation_;

    static double CalcPercentage(double time);
    static double CalcPosTapeHead(double time);
};