#pragma once

#include <iostream>
#include <vector>

#include "window.h"
#include "general_data.h"


class WindowManager {
public:
    void AddWindow(Window win) {
        windows_.push_back(std::move(win));
    }

    void Start() {
        sf::RenderWindow win(sf::VideoMode(win_size.x, win_size.y), "TuringMachineSimulator", sf::Style::Default);

        win.setFramerateLimit(360);

        while (win.isOpen()) {
            sf::Event event;
            while (win.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed: {
                        win.close();
                        break;
                    }
                    case sf::Event::Resized: {
                        win_size = { (float)event.size.width, (float)event.size.height };
                        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                        win.setView(sf::View(visibleArea));
                        win.clear(background_color);
                        win.draw(windows_[active_]);
                        win.display();
                        break;
                    }
                    case sf::Event::TextEntered:
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) &&
                            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
                            ++active_;
                            active_ %= windows_.size();
                            break;
                        }
                    default: {
                        if (windows_[active_].ProcessEvent(event, sf::Vector2f(win.getSize()))) {
                            win.clear(background_color);
                            win.draw(windows_[active_]);
                            win.display();
                        }
                    }
                }
            }
        }
    }

private:
    size_t active_ = 0;
    std::vector<Window> windows_;
};
