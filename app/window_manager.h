#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>

#include "window.h"
#include "general_data.h"



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
            win.setVerticalSyncEnabled(true);
            while (true) {
                if (close) {
                    win.close();
                    return;
                }
                win.clear(background_color);
                win.draw(windows_[active_]);
                win.display();
            }
        });

        sf::Event event;
        while (win.isOpen() && win.waitEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
                ++active_;
                active_ %= windows_.size();
                break;
            }
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
                default: {
                    windows_[active_].ProcessEvent(event);
                    break;
                }
            }
        }
    }

private:
    size_t active_ = 0;
    std::vector<Window> windows_;
};
