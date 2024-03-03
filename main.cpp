#include "app/window.h"
#include "elements/table.h"
#include "app/window_manager.h"
#include "app/general_data.h"
#include "elements/tape.h"
#include "elements/tape_head.h"
#include "elements/rounded_rectangle.h"

sf::Font font = LoadFont("../app/3270NerdFontMono-Regular.ttf");
float letter_size = 20.f;
sf::Color fill_color = sf::Color(187, 189, 246);
sf::Color outline_color = sf::Color(114, 114, 126);
sf::Color text_color = sf::Color(0, 0, 0);
sf::Color background_color = sf::Color(152, 147, 218);
sf::Vector2f win_size = {1500, 1000};
sf::String lambda(L"\u03BB");
long animation_time = 1500;

int main() {
    TuringMachine machine;
    machine.AddColumn(lambda[0]);
    machine.AddColumn('a');
    machine.AddColumn('b');
    machine.AddLine();
    machine.AddLine();
    machine.AddLine();

    WindowManager wm;

    {
        Window win_tape;
        std::unique_ptr<Tape> tape = std::make_unique<Tape>(machine, 100);
        std::unique_ptr<TapeHead> tape_head = std::make_unique<TapeHead>(160);
        auto cb_move_l = [&tape_ref = *tape, &tape_head_ref = *tape] () {
            tape_ref.MoveLeft();
            tape_head_ref.MoveLeft();
        };
        auto cb_move_r = [&tape_ref = *tape, &tape_head_ref = *tape_head] () {
            tape_ref.MoveRight();
            tape_head_ref.MoveRight();
        };
        machine.SetCallBacks(cb_move_r, cb_move_l);
        win_tape.AddElement(std::move(tape));
        win_tape.AddElement(std::move(tape_head));

        std::unique_ptr<ButtonWithTextRelativePos> next_step = std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(200, 300), sf::Vector2f(100, 50),
                                                                                                      "Next step", [&machine] () { machine.Do1Tick(); });
        win_tape.AddElement(std::move(next_step));

        wm.AddWindow(std::move(win_tape));
    }

    {
        Window win_table;
        win_table.AddElement(std::make_unique<Table>(sf::Vector2f(10, 10), sf::Vector2f(800, 980), sf::Vector2f(1000, 1000), machine));

        std::unique_ptr<InputField<RoundedRectangleShape<10>>> ptr = std::make_unique<InputField<RoundedRectangleShape<10>>>(sf::Vector2f(820, 10), sf::Vector2f(170, 30));
        win_table.AddElement(std::move(ptr));

        wm.AddWindow(std::move(win_table));
    }

    wm.Start();
}