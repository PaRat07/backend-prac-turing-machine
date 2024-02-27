#include "window.h"
#include "table.h"
#include "window_manager.h"
#include "general_data.h"

sf::Font font = LoadFont("../JetBrainsMono-Medium.ttf");
float letter_size = 20.f;
sf::Color fill_color = sf::Color(187, 189, 246);
sf::Color outline_color = sf::Color(114, 114, 126);
sf::Color text_color = sf::Color(0, 0, 0);
sf::Color background_color = sf::Color(152, 147, 218);
sf::Vector2f win_size = {1500, 1000};
uint16_t fps_limit = 60;

int main() {
    TuringMachine machine;
    char sym = '0';
    for (int i = 0; i < 20; ++i) {
        machine.AddColumn(++sym);
        machine.AddLine();
    }

    Window win;
    win.AddElement(std::make_unique<Table>(sf::Vector2f(0, 0), sf::Vector2f(800, 800), sf::Vector2f(1000, 1000), machine));

    WindowManager wm;
    wm.AddWindow(std::move(win));

    wm.Start();
}