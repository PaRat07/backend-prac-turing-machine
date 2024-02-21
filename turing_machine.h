#pragma once

#include <map>
#include <string>
#include <vector>
#include <SFML/System.hpp>

class TuringMachine {
public:
    struct ValueOfTable {
        ValueOfTable(int cur_q)
                : to_write('#')
                , move(Move::DONT_MOVE)
                , q(cur_q)
        {}

        ValueOfTable(std::string_view val, int cur_q) {
            if (val[0] != ',') {
                to_write = val[0];
                val.remove_prefix(2);
            } else {
                to_write = '#';
                val.remove_prefix(1);
            }

            if (val[0] != ',') {
                move = (val[0] == 'L' ? Move::MOVE_LEFT : Move::MOVE_RIGHT);
                val.remove_prefix(2);;
            } else {
                move = Move::DONT_MOVE;
                val.remove_prefix(1);
            }

            if (!val.empty()) {
                val.remove_prefix(1);
                q = std::stoi(std::string(val));
            } else {
                q = -1;
            }
        }

        std::string ToStr() const {
            std::string ans;
            ans += to_write;
            switch (move) {
                case Move::MOVE_LEFT:
                    ans.push_back('L');
                    break;
                case Move::MOVE_RIGHT:
                    ans.push_back('R');
                    break;
            }
            ans.push_back(',');
            ans += "q" + std::to_string(q);
            return ans;
        }

        char to_write;
        enum class Move {
            DONT_MOVE = 0,
            MOVE_LEFT = 1,
            MOVE_RIGHT = 2
        };
        Move move;
        int q;
    };

    std::string GetFrom(int pos, int len) {
        std::string ans(len, '#');
        auto it = tape_.lower_bound(pos);
        for (int i = pos; i < pos + len; ++i) {
            if (it->first < i) ++it;

            if (it->first == i) {
                ans[i] = it->second;
            }
        }
        return ans;
    }

    void Write(int pos, char sym) {
        if (sym == '#') {
            tape_.erase(pos);
        } else {
            tape_[pos] = sym;
        }
    }

    void SetTableValue(sf::Vector2f pos, ValueOfTable val) {
        table_[pos.x][pos.y] = val;
    }

    void AddLine() {
        qs_.push_back(qs_.size());
        for (int i = 0; i < table_size_.x; ++i) {
            table_[i].emplace_back(table_size_.y);
        }
    }

    void AddColumn(char sym) {
        syms_.push_back(sym);
        table_.emplace_back(table_size_.y, 0);
        int cur_q = 0;
        std::for_each(table_.back().begin(), table_.back().end(), [&cur_q] (ValueOfTable &val) {
            val = ValueOfTable(++cur_q);
        });
    }

    const std::vector<std::vector<ValueOfTable>> &GetTable() const {
        return table_;
    }

    std::vector<std::string> GetSyms() const {
        std::vector<std::string> ans(qs_.size());
        std::transform(syms_.begin(), syms_.end(), ans.begin(), [] (char sym) {
            return std::string(1, sym);
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

private:
    // table_[x][y], x - sym, y - q
    sf::Vector2f table_size_;
    std::string syms_;
    std::vector<int> qs_;
    std::map<int,char> tape_;
    std::vector<std::vector<ValueOfTable>> table_;


    char Read(int pos) {
        return (tape_.count(pos) ? tape_[pos] : '#');
    }
};
