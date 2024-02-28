#pragma once

#include <map>
#include <string>
#include <vector>
#include <SFML/System.hpp>
#include <iostream>

class TuringMachine {
public:
    struct ValueOfTable {
        ValueOfTable(int cur_q)
                : to_write('#')
                , move(Move::DONT_MOVE)
                , q(cur_q)
                , view("#,,q" + std::to_string(cur_q))
        {
        }

        ValueOfTable(std::string_view val) {
            view = val;
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
            return view;
        }

        char to_write;
        enum class Move {
            DONT_MOVE = 0,
            MOVE_LEFT = 1,
            MOVE_RIGHT = 2
        };
        Move move;
        int q;
        std::string view;
    };

    std::string GetFrom(int pos, int len) {
        std::string ans(len, '#');
        for (int i = pos; i < pos + len; ++i) {
            ans[i - pos] = Read(i);
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

    void AddColumn(char sym) {
        syms_.push_back(sym);
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
        std::vector<std::string> ans(syms_.size());
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

    sf::Vector2i Size() const {
        return {(int)syms_.size(), (int)qs_.size()};
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

        syms_.erase(syms_.begin() + ind);
        table_.erase(table_.begin() + ind);
        std::cerr << "Erased" << std::endl;
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
