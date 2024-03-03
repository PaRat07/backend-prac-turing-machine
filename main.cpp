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
sf::Vector2f win_size = {1000, 1000};
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

    Window win_tape;
    {
        {
            std::unique_ptr<Tape> tape = std::make_unique<Tape>(machine, 100);
            std::unique_ptr<TapeHead> tape_head = std::make_unique<TapeHead>(160);
            auto cb_move_l = [&tape_ref = *tape, &tape_head_ref = *tape]() {
                tape_ref.MoveLeft();
                tape_head_ref.MoveLeft();
            };
            auto cb_move_r = [&tape_ref = *tape, &tape_head_ref = *tape_head]() {
                tape_ref.MoveRight();
                tape_head_ref.MoveRight();
            };
            machine.SetCallBacks(cb_move_r, cb_move_l);
            win_tape.AddElement(std::move(tape));
            win_tape.AddElement(std::move(tape_head));
        }

        win_tape.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                                    sf::Vector2f(10, 300), sf::Vector2f(100, 50),
                                    "Next step", [&machine]() { machine.Do1Tick(); }));

        win_tape.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                sf::Vector2f(10, 400), sf::Vector2f(100, 50),
                "-", [&] () { animation_time += 100; }));

        win_tape.AddElement(std::make_unique<ButtonWithTextRelativePos>(
                sf::Vector2f(10, 450), sf::Vector2f(100, 50),
                "+", [&] () { animation_time -= 100; animation_time = std::max(animation_time, 0l); }));
    }


    Window win_table;
    {
        win_table.AddElement(std::make_unique<Table>(sf::Vector2f(10, 10), sf::Vector2f(800, 980), machine));

        std::unique_ptr<InputField<RoundedRectangleShape<10>>> scan = std::make_unique<InputField<RoundedRectangleShape<10>>>(sf::Vector2f(820, 10),
                                                                                                                              sf::Vector2f(170, 40));

        auto sym_adder = [&scanner = *scan, &machine] () {
            machine.AddColumn(scanner.GetText()[0]);
        };

        win_table.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(820, 60),
                                                                                sf::Vector2f(80, 40),
                                                                                 "Add Sym",
                                                                                 sym_adder));

        win_table.AddElement(std::make_unique<ButtonWithTextRelativePos>(sf::Vector2f(910, 60),
                                                                         sf::Vector2f(80, 40),
                                                                         "Add Q",
                                                                         std::bind(&TuringMachine::AddLine, &machine)));
        win_table.AddElement(std::move(scan));
    }


    WindowManager wm;
    wm.AddWindow(std::move(win_table));
    wm.AddWindow(std::move(win_tape));

    wm.Start();
}