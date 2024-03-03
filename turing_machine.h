#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <optional>

#include "app/general_data.h"

class TuringMachine {
public:
    struct ValueOfTable {
        ValueOfTable(int cur_q)
                : to_write('\0')
                , move(Move::DONT_MOVE)
                , q(cur_q)
                , view(lambda + ",,q" + std::to_string(cur_q))
        {
        }

        ValueOfTable(std::string_view val, int q_val) {
            if (val[0] != ',') {
                if (val.substr(0, 2) == "ld") {
                    to_write = lambda;
                    val.remove_prefix(3);
                } else {
                    to_write = val[0];
                    val.remove_prefix(2);
                }
            } else {
                to_write = std::nullopt;
                val.remove_prefix(1);
            }
            view += sf::String((to_write ? *to_write : "") + ',');

            if (val[0] != ',') {
                view += val[0];
                move = (val[0] == 'L' ? Move::MOVE_LEFT : Move::MOVE_RIGHT);
                val.remove_prefix(2);;
            } else {
                move = Move::DONT_MOVE;
                val.remove_prefix(1);
            }
            view += ',';

            if (!val.empty()) {
                val.remove_prefix(1);
                q = std::stoi(std::string(val));
            } else {
                q = q_val;
            }
            view += "q" + std::to_string(q);
        }

        sf::String ToStr() const {
            return view;
        }

        std::optional<sf::String> to_write;
        enum class Move {
            DONT_MOVE = 0,
            MOVE_LEFT = 1,
            MOVE_RIGHT = 2
        };
        Move move;
        int q;
        sf::String view;
    };

    sf::String GetFrom(int pos, int len) {
        sf::String ans;
        for (int i = pos; i < pos + len; ++i) {
            ans[i - pos] = Read(i)[0];
        }
        return ans;
    }

    void Write(int pos, sf::String sym) {
        if (sym == lambda) {
            tape_.erase(pos);
        } else {
            tape_[pos] = sym;
        }
    }

    void SetTableValue(sf::Vector2i pos, ValueOfTable val) {
        table_[pos.x][pos.y] = val;
    }

    void AddLine() {
        qs_.push_back(qs_.size());
        for (int i = 0; i < table_size_.x; ++i) {
            table_[i].emplace_back(table_size_.y);
        }
        ++table_size_.y;
    }

    void AddColumn(sf::Uint32 sym) {
        syms_ += sym;
        table_.emplace_back(table_size_.y, 0);
        int cur_q = 0;
        std::for_each(table_.back().begin(), table_.back().end(), [&cur_q] (ValueOfTable &val) {
            val = ValueOfTable(++cur_q);
        });
        ++table_size_.x;
    }

    const std::vector<std::vector<ValueOfTable>> &GetTable() const {
        return table_;
    }

    std::vector<std::string> GetSyms() const {
        std::vector<std::string> ans(syms_.getSize());
        std::transform(syms_.begin(), syms_.end(), ans.begin(), [] (sf::Uint32 sym) {
            return sf::String(sym);
        });
        return ans;
    }

    std::vector<std::string> GetQs() const {
        std::vector<std::string> ans(qs_.size());
        std::transform(qs_.begin(), qs_.end(), ans.begin(), [] (int q) {
            return "q" + std::to_string(q);
        });
        return ans;
    }

    sf::Vector2i Size() const {
        return {(int)syms_.getSize(), (int)qs_.size()};
    }

    void EraseQ(std::string q) {
        int to_del = std::stoi(q.substr(1));
        size_t ind = std::find(qs_.begin(), qs_.end(), to_del) - qs_.begin();
        if (ind == qs_.size()) return;
        qs_.erase(qs_.begin() + ind);
        std::for_each(table_.begin(), table_.end(), [ind] (std::vector<ValueOfTable> &v) {
            v.erase(v.begin() + ind);
        });
    }

    void EraseSym(char to_del) {
        size_t ind = syms_.find(to_del);
        if (ind == std::string::npos) return;

        syms_.erase(ind);
        table_.erase(table_.begin() + ind);
    }

    sf::String Read(int pos) {
        return (tape_.count(pos) ? tape_[pos] : lambda);
    }

    void SetCallBacks(std::function<void()> cb_move_r, std::function<void()> cb_move_l) {
        cb_move_l_ = std::move(cb_move_l);
        cb_move_r_ = std::move(cb_move_r);
    }

    void Do1Tick() {
        const ValueOfTable &val = table_[syms_.find(Read(cur_pos_in_tape))][cur_q_];
        if (val.to_write) {
            tape_[cur_pos_in_tape] = *val.to_write;
        }

        if (val.move == ValueOfTable::Move::MOVE_LEFT) {
            --cur_pos_in_tape;
            cb_move_l_();
        } else if (val.move == ValueOfTable::Move::MOVE_RIGHT) {
            ++cur_pos_in_tape;
            cb_move_r_();
        }

        cur_q_ = val.q;
    }

private:
    // table_[x][y], x - sym, y - q
    int cur_pos_in_tape = 0;
    size_t cur_q_ = 0;
    std::function<void()> cb_move_l_, cb_move_r_;
    sf::Vector2f table_size_;
    sf::String syms_;
    std::vector<int> qs_;
    std::map<int, sf::String> tape_;
    std::vector<std::vector<ValueOfTable>> table_;
};
