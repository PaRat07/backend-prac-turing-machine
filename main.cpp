#include "app/window.h"
#include "elements/table.h"
#include "app/window_manager.h"
#include "app/general_data.h"
#include "elements/tape.h"

sf::Font font = LoadFont("../app/JetBrainsMono-Medium.ttf");
float letter_size = 20.f;
sf::Color fill_color = sf::Color(187, 189, 246);
sf::Color outline_color = sf::Color(114, 114, 126);
sf::Color text_color = sf::Color(0, 0, 0);
sf::Color background_color = sf::Color(152, 147, 218);
sf::Vector2f win_size = {1500, 1000};

int main() {
    TuringMachine machine;

    char sym = '0';
    for (int i = 0; i < 52; ++i) {
        machine.AddColumn(++sym);
        machine.AddLine();
    }

    WindowManager wm;
    {
        Window win_table;
        win_table.AddElement(
                std::make_unique<Table>(sf::Vector2f(0, 0), sf::Vector2f(800, 800), sf::Vector2f(1000, 1000), machine));

        wm.AddWindow(std::move(win_table));
    }

    {
        Window win_tape;
        win_tape.AddElement(std::make_unique<Tape>(machine, 100));

        wm.AddWindow(std::move(win_tape));
    }

    wm.Start();
}