#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>


class AbstractElement : public sf::Drawable {
public:
    virtual bool ProcessEvent(sf::Event event) = 0;
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

    bool ProcessEvent(sf::Event event, sf::Vector2f win_size) {
        // for (auto &i : elements_) {
        //     i->ProcessEvent(event, target);
        // }
        bool ans = false;
        std::for_each(elements_.begin(), elements_.end(), [event, &ans] (const std::unique_ptr<AbstractElement> &ptr) {
            ans |= ptr->ProcessEvent(event);
        });
        return ans;
    }
private:
    std::vector<std::unique_ptr<AbstractElement>> elements_;
};
