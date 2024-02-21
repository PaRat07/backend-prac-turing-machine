#include <functional>

#include "SFML/Graphics.hpp"

#include "rounded_rectangle.h"
#include "general_data.h"
#include "window.h"

class InputField : public AbstractElement {
public:
    InputField(sf::Vector2f pos, sf::Vector2f size, const sf::RenderTarget &target);

    void ProcessEvent(sf::Event event, const sf::RenderTarget &target) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void Write(sf::Uint32 event);

    const std::string &GetText() const {
        return data_;
    }

private:
    sf::Vector2f pos_, size_;
    std::string data_;
    bool active_ = false;
};

class OutputField : public AbstractElement {
public:
    OutputField(sf::Vector2f pos, sf::Vector2f size, const sf::RenderTarget &target);

    void ProcessEvent(sf::Event event, sf::RenderTarget &target);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void SetText(std::string s);

private:
    sf::Vector2f pos_, size_;
    std::string data_;
    bool active_ = false;
};
