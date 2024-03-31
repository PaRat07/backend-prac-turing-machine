#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <thread>
#include <set>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#include "app/general_data.h"
#include "elements/table.h"

template<typename T>
using ordered_set = __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;

class TuringMachine : public Table {
public:
    struct ValueOfTable {
        ValueOfTable();

        ValueOfTable(std::string_view val);

        sf::String ToStr() const {
            sf::String ans;
            if (to_write.has_value()) {
                ans += *to_write;
            }
            ans += ",";
            switch (move) {
                case Move::MOVE_LEFT:
                    ans += "L";
                    break;
                case Move::MOVE_RIGHT:
                    ans += "R";
                    break;
            }
            ans += ",";
            if (q.has_value()) {
                ans += "q" + std::to_string(*q);
            }
            if (term) {
                ans += "!";
            }
            return ans;
        }
        enum class Move {
            DONT_MOVE = 0,
            MOVE_LEFT = 1,
            MOVE_RIGHT = 2
        };

        std::optional<sf::Uint32> to_write;
        Move move;
        std::optional<int> q;
        bool term;
    };

    sf::String GetFrom(int pos, int len);

    void Write(int pos, sf::Uint32 sym);

    void SetValue(sf::Vector2i pos, sf::String val) override;

    void AddLine();

    void AddColumn(sf::Uint32 sym);

    sf::String GetValue(sf::Vector2i pos) const override;

    sf::Vector2i Size() const override;

    void EraseLine(int pos) override;

    void EraseColumn(int pos) override;

    sf::Uint32 Read(int pos);

    void SetCallBacks(std::function<void()> cb_move_r, std::function<void()> cb_move_l);

    void Do1Tick();

    bool CorrSym(sf::Uint32 sym) const;

    void Reset();

    sf::String GetColumnName(int pos) const override;

    sf::String GetLineName(int pos) const override;

private:
    // table_[x][y], x - sym, y - q
    int cur_pos_in_tape = 0;
    size_t cur_q_ = 0;
    size_t last_q_ = -1;
    std::function<void()> cb_move_l_, cb_move_r_;
    ordered_set<sf::Uint32> syms_;
    ordered_set<int> qs_;
    std::map<int, sf::Uint32> tape_;
    std::map<sf::Uint32, std::map<int, ValueOfTable>> table_;
    std::optional<std::map<int, sf::Uint32>> reserved_tape_;
    mutable std::mutex change_table_;
    bool ended_ = false;

    bool CanDoTick() const;
};
