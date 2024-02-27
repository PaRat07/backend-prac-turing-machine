#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <future>
#include <optional>
#include <cmath>

#include "center_positioned_string.h"
#include "turing_machine.h"
#include "window.h"
#include "general_data.h"
#include "rounded_rectangle.h"
#include "inoutput_field.h"
#include "button.h"

class Table : public AbstractElement {
public:
    static sf::Vector2i cell_size;

    Table(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f win_size, TuringMachine &machine)
        : machine_(machine)
        , pos_(pos.x / win_size.x, pos.y / win_size.y)
        , size_(size.x / win_size.x, size.y / win_size.y)
    {}

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        buttons_.clear();
        for (int i = 0; i < machine_.Size().x; ++i) {
            char to_delete = machine_.GetSyms()[i][0];
            auto eraser = [&, to_delete] () {
                std::cerr << "Called" << std::endl;
                machine_.EraseSym(to_delete);
            };
            ButtonWithTextAbsPos erase_sym(sf::Vector2f((i + 0.5) * cell_size.x + pos_in_.x, 12 + cell_size.y / 2), sf::Vector2f(40, 30), "del", eraser);
            buttons_.push_back(erase_sym);
        }

        sf::Vector2i real_size(size_.x * target.getSize().x, size_.y * target.getSize().y);

        // frame
        {
            sf::RoundedRectangleShape frame{sf::Vector2f(real_size.x - 4, real_size.y - 4)};
            frame.setPosition(sf::Vector2f(pos_.x * target.getSize().x + 2, pos_.y * target.getSize().y + 2));
            frame.setRoundRadius(10);
            frame.setFillColor(background_color);
            frame.setOutlineThickness(2);
            target.draw(frame);
            sf::RectangleShape table_border(sf::Vector2f(real_size.x - 24, real_size.y - 24));
            table_border.setPosition(sf::Vector2f(pos_.x * target.getSize().x + 14, pos_.y * target.getSize().y + 14));
            table_border.setFillColor(sf::Color::Transparent);
            table_border.setOutlineThickness(2);
            table_border.setOutlineColor(outline_color);
            target.draw(table_border);
        }

        // net + vals
        {
            sf::RenderTexture net_texture;
            sf::Sprite net_sprite;
            net_texture.create(real_size.x - cell_size.x - 24, real_size.y - cell_size.y * 2 - 24);
            net_texture.clear(background_color);
            sf::RectangleShape line(sf::Vector2f(real_size.x - 24, 2));
            line.setFillColor(outline_color);
            for (int i = pos_in_.y % cell_size.y; i < real_size.y; i += cell_size.y) {
                line.setPosition(0, i);
                net_texture.draw(line);
            }
            net_texture.display();
            net_sprite.setPosition(12 + cell_size.x, cell_size.y + 12);

            line.setSize(sf::Vector2f(2, real_size.y - 24));
            for (int i = pos_in_.x % cell_size.x; i < real_size.x; i += cell_size.x) {
                line.setPosition(i, 0);
                net_texture.draw(line);
            }

            {
                CenterPositionedString str;
                for (int i = -pos_in_.x / cell_size.x; i * cell_size.x + pos_in_.x < real_size.x && i < machine_.Size().x; ++i) {
                    for (int j = -pos_in_.y / cell_size.y; j * cell_size.y + pos_in_.y < real_size.y && j < machine_.Size().y; ++j) {
                        str.setPosition(i * cell_size.x + cell_size.x / 2 + pos_in_.x,
                                        j * cell_size.y + cell_size.y / 2 + pos_in_.y);
                        str.setString(machine_.GetTable()[i][j].ToStr());
                        net_texture.draw(str);
                    }
                }
            }
            if (field.has_value()) {
                net_texture.draw(*field);
            }
            net_texture.display();
            net_sprite.setTexture(net_texture.getTexture());
            net_sprite.setPosition(cell_size.x + 12, cell_size.y * 2 + 12);
            target.draw(net_sprite);
        }

        // syms
        {
            std::vector<std::string> syms_vals = machine_.GetSyms();
            sf::RenderTexture syms;
            syms.create(real_size.x - cell_size.x - 24, cell_size.y * 2);
            syms.clear(sf::Color::Transparent);
            CenterPositionedString str;
            for (int i = -pos_in_.x / cell_size.x; i < syms_vals.size() && i * cell_size.x + pos_in_.x < real_size.x; ++i) {
                str.setString(syms_vals[i]);
                str.setPosition(sf::Vector2f(i * cell_size.x + cell_size.x / 2 + pos_in_.x, cell_size.y * 1.5));
                syms.draw(str);
                syms.draw(buttons_[i]);
            }
            sf::RectangleShape line(sf::Vector2f(2, cell_size.y * 2));
            line.setFillColor(outline_color);

            for (int i = pos_in_.x % cell_size.x; i < real_size.x; i += cell_size.x) {
                line.setPosition(i, 0);
                syms.draw(line);
            }
            syms.display();
            sf::Sprite sprite;
            sprite.setPosition(cell_size.x + 12, 12);
            sprite.setTexture(syms.getTexture());
            target.draw(sprite);
        }

        // qs
        {
            std::vector<std::string> qs_vals = machine_.GetQs();
            sf::RenderTexture qs;
            qs.create(cell_size.x, real_size.y - cell_size.y * 2 - 24);
            qs.clear(sf::Color::Transparent);
            CenterPositionedString str;
            for (int i = -pos_in_.y / cell_size.y; i < qs_vals.size() && i * cell_size.y + pos_in_.y < real_size.y; ++i) {
                str.setString(qs_vals[i]);
                str.setPosition(sf::Vector2f(cell_size.x / 2, i * cell_size.y + cell_size.y / 2 + pos_in_.y));
                qs.draw(str);
            }
            sf::RectangleShape line(sf::Vector2f(cell_size.x, 2));
            line.setFillColor(outline_color);

            for (int i = pos_in_.y % cell_size.y; i < real_size.y; i += cell_size.y) {
                line.setPosition(0, i);
                qs.draw(line);
            }
            qs.display();
            sf::Sprite sprite;
            sprite.setPosition(12,  cell_size.y * 2 + 12);
            sprite.setTexture(qs.getTexture());
            target.draw(sprite);
        }

        // ver line(not texture)
        {
            sf::RectangleShape legend_border(sf::Vector2f(2, real_size.y - 24));
            legend_border.setFillColor(outline_color);
            target.draw(legend_border);
            legend_border.setSize(sf::Vector2f(real_size.x - 24, 2));
            legend_border.setPosition(sf::Vector2f(pos_.x * target.getSize().x + 12, pos_.y * target.getSize().y + 12 + cell_size.y * 2));
            target.draw(legend_border);
        }

        // hor line(not texture)
        {
            sf::RectangleShape legend_border(sf::Vector2f(2, real_size.y - 24));
            legend_border.setFillColor(outline_color);
            target.draw(legend_border);
            legend_border.setSize(sf::Vector2f(real_size.x - 24, 2));
            legend_border.setPosition(sf::Vector2f(pos_.x * target.getSize().x + 12, pos_.y * target.getSize().y + 12 + cell_size.y * 2));
            target.draw(legend_border);
        }
    }

    void ProcessEvent(sf::Event event) override {
        switch (event.type) {
            case sf::Event::EventType::MouseWheelScrolled: {
                if (event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel) {
                    pos_in_.y += event.mouseWheelScroll.delta * 3;
                    pos_in_.y = std::max(pos_in_.y, (int) (-GetTableSize().y + size_.y * win_size.y - 24));
                    pos_in_.y = std::min(pos_in_.y, 0);
                } else {
                    pos_in_.x += event.mouseWheelScroll.delta * 3;
                    pos_in_.x = std::max(pos_in_.x, (int) (-GetTableSize().x + size_.x * win_size.x - 24));
                    pos_in_.x = std::min(pos_in_.x, 0);
                }
                field->SetPosition(sf::Vector2f(active_pos_->x * cell_size.x + pos_in_.x + 2,
                                                active_pos_->y * cell_size.y + pos_in_.y + 2));
            }
            case sf::Event::EventType::MouseButtonPressed: {
                sf::Event ev_for_buts = event;
                ev_for_buts.mouseButton.y -= 12;
                std::for_each(buttons_.begin(), buttons_.end(), [&ev_for_buts] (ButtonWithTextAbsPos & but) {
                    but.ProcessEvent(ev_for_buts);
                });

                sf::Vector2f pos(event.touch.x / win_size.x, event.touch.y / win_size.y);
                if (std::abs(pos_.x + size_.x / 2 - pos.x) <= size_.x / 2 &&
                    std::abs(pos_.x + size_.x / 2 - pos.x) <= size_.x / 2) {
                    active_pos_.emplace((event.mouseButton.x - pos_in_.x - 14) / cell_size.x - 1,
                                        (event.mouseButton.y - pos_in_.y - 14 - cell_size.y) / cell_size.y - 1);
                    field.emplace(sf::Vector2f(active_pos_->x * cell_size.x + pos_in_.x + 2,
                                                    active_pos_->y * cell_size.y + pos_in_.y + 2),
                                  sf::Vector2f(cell_size.x - 2, cell_size.y - 2));
                    field->Activate();
                }
            }
            case sf::Event::TextEntered:
                if (field.has_value()) {
                    if (event.text.unicode == 13) {
                        machine_.SetTableValue(*active_pos_, TuringMachine::ValueOfTable(field->GetText()));
                    }
                    else {
                        field->ProcessEvent(event);
                    }
                }
                break;
        }
    }

private:
    TuringMachine &machine_;
    sf::Vector2f pos_, size_;
    sf::Vector2i pos_in_ = {0,0};
    std::optional<sf::Vector2i> active_pos_;
    mutable std::vector<ButtonWithTextAbsPos> buttons_;
    std::optional<InputField<sf::RectangleShape>> field;

    sf::Vector2i GetTableSize() const {
        return { cell_size.x * (machine_.Size().x + 1),cell_size.y * (machine_.Size().y + 1) };
    }
};
