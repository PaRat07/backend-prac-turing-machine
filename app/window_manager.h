#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>

#include "window.h"
#include "../general_data.h"



class WindowManager {
public:
    void AddWindow(Window win) {
        windows_.push_back(std::move(win));
    }

    void Start() {
        sf::RenderWindow win(sf::VideoMode(win_size.x, win_size.y), "TuringMachineSimulator", sf::Style::Default);


        win.setActive(false);
        bool close = false;
        auto displayer = std::async([&] () {
            while (win.isOpen()) {
                if (close) {
                    win.close();
                    return;
                }
                win.clear(background_color);
                win.draw(windows_[active_]);
                win.display();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps_limit));
            }
        });
        sf::Event event;
        while (win.isOpen() && win.waitEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    close = true;
                    displayer.wait();
                    break;
                }
                case sf::Event::Resized: {
                    win_size = {(float) event.size.width, (float) event.size.height};
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    win.setView(sf::View(visibleArea));
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
                    windows_[active_].ProcessEvent(event, sf::Vector2f(win.getSize()));
                }
            }
        }
    }

private:
    size_t active_ = 0;
    std::vector<Window> windows_;
};
