#pragma once

#include <iostream>

#include "SFML/Graphics.hpp"
#include <future>
#include <optional>
#include <cmath>

#include "center_positioned_string.h"
#include "../turing_machine.h"
#include "../app/window.h"
#include "../app/general_data.h"
#include "rounded_rectangle.h"
#include "inoutput_field.h"
#include "button.h"

class Table : public AbstractElement {
public:
    static sf::Vector2i cell_size;

    Table(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f win_size, TuringMachine &machine);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void ProcessEvent(sf::Event event) override;

private:
    TuringMachine &machine_;
    sf::Vector2f pos_, size_;
    sf::Vector2i pos_in_ = {0,0};
    std::optional<sf::Vector2i> active_pos_;
    mutable std::vector<ButtonWithTextAbsPos> buttons_syms_;
    mutable std::mutex buttons_syms_mutex_;
    mutable std::vector<ButtonWithTextAbsPos> buttons_qs_;
    mutable std::mutex buttons_qs_mutex_;
    std::optional<InputField<sf::RectangleShape>> field;

    sf::Vector2i GetTableSize() const;
};
