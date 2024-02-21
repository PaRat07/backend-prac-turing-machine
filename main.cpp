#include "window.h"
#include "table.h"
#include "window_manager.h"

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