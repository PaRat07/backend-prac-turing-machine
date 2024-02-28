#pragma once

#include <memory>
#include <vector>

#include "SFML/Graphics.hpp"


class AbstractElement : public sf::Drawable {
public:
    virtual void ProcessEvent(sf::Event event) = 0;
    virtual ~AbstractElement() = default;
};

class Window : public sf::Drawable {
public:
    Window(const Window &other) = delete;
    Window(Window &&other) = default;
    Window() = default;

    template<class Elem>
    void AddElement(std::unique_ptr<Elem> new_elem) {
        elements_.emplace_back(std::move(new_elem));
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        // for (auto &i : elements_) {
        //     target.draw(*i);
        // }

        std::for_each(elements_.begin(), elements_.end(), [&target] (const std::unique_ptr<AbstractElement> &ptr) {
            target.draw(*ptr);
        });
    }

    void ProcessEvent(sf::Event event) {
        // for (auto &i : elements_) {
        //     i->ProcessEvent(event, target);
        // }

        std::for_each(elements_.begin(), elements_.end(), [event] (const std::unique_ptr<AbstractElement> &ptr) {
            ptr->ProcessEvent(event);
        });
    }
private:
    std::vector<std::unique_ptr<AbstractElement>> elements_;
};
