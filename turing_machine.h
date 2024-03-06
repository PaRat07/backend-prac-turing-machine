#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <thread>

#include "app/general_data.h"

class TuringMachine {
public:
    struct ValueOfTable {
        ValueOfTable(int cur_q)
                : to_write(std::nullopt)
                , move(Move::DONT_MOVE)
                , q(cur_q)
                , view(lambda + ",,q" + std::to_string(cur_q))
                , term(false)
        {
        }

        ValueOfTable(std::string_view val, int q_val) {
            if (val[0] != ',') {
                if (val[0] == '!') {
                    term = true;
                    to_write = std::nullopt;
                    move = Move::DONT_MOVE;
                    view = "!";
                    q = q_val;
                    return;
                } else if (val.substr(0, 2) == "ld") {
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
                if (val[0] == '!') {
                    term = true;
                    view += "!";
                    move = Move::DONT_MOVE;
                    q = q_val;
                    return;
                }
                view += val[0];
                move = (val[0] == 'L' ? Move::MOVE_LEFT : Move::MOVE_RIGHT);
                val.remove_prefix(2);
            } else {
                move = Move::DONT_MOVE;
                val.remove_prefix(1);
            }
            view += ',';

            if (!val.empty()) {
                if (val[0] == '!') {
                    term = true;
                    q = q_val;
                    return;
                }
                val.remove_prefix(1);
                q = std::stoi(std::string(val));
            } else {
                q = q_val;
            }
            view += "q" + std::to_string(q);
            term = false;
        }

        sf::String ToStr() const {
            return view;
        }
        enum class Move {
            DONT_MOVE = 0,
            MOVE_LEFT = 1,
            MOVE_RIGHT = 2
        };

        std::optional<sf::String> to_write;
        Move move;
        int q;
        sf::String view;
        bool term;
    };

    sf::String GetFrom(int pos, int len) {
        sf::String ans;
        for (int i = pos; i < pos + len; ++i) {
            ans[i - pos] = Read(i)[0];
        }
        return ans;
    }

    void Write(int pos, sf::String sym) {
        ended_ = false;
        reserved_tape_.reset();
        if (sym == lambda) {
            tape_.erase(pos);
        } else {
            tape_[pos] = sym;
        }
    }

    void SetTableValue(sf::Vector2i pos, ValueOfTable val) {
        ended_ = false;
        reserved_tape_.reset();
        table_[pos.x][pos.y] = val;
    }

    void AddLine() {
        ended_ = false;
        reserved_tape_.reset();
        qs_.push_back(qs_.size());
        std::lock_guard guard(change_table_);
        for (int i = 0; i < table_size_.x; ++i) {
            table_[i].emplace_back(table_size_.y);
        }
        ++table_size_.y;
    }

    void AddColumn(sf::Uint32 sym) {
        ended_ = false;
        reserved_tape_.reset();
        syms_ += sym;
        table_.emplace_back(table_size_.y, 0);
        int cur_q = 0;
        std::lock_guard guard(change_table_);
        std::for_each(table_.back().begin(), table_.back().end(), [&cur_q] (ValueOfTable &val) {
            val = ValueOfTable(++cur_q);
        });
        ++table_size_.x;
    }

    const std::vector<std::vector<ValueOfTable>> &GetTable() const {
        return table_;
    }

    std::vector<std::string> GetSyms() const {
        std::lock_guard guard(change_table_);
        std::vector<std::string> ans(syms_.getSize());
        std::transform(syms_.begin(), syms_.end(), ans.begin(), [] (sf::Uint32 sym) {
            return sf::String(sym);
        });
        return ans;
    }

    std::vector<std::string> GetQs() const {
        std::lock_guard guard(change_table_);
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
        ended_ = false;
        reserved_tape_.reset();
        int to_del = std::stoi(q.substr(1));
        size_t ind = std::find(qs_.begin(), qs_.end(), to_del) - qs_.begin();
        if (ind == qs_.size()) return;
        std::lock_guard guard(change_table_);
        qs_.erase(qs_.begin() + ind);
        std::for_each(table_.begin(), table_.end(), [ind] (std::vector<ValueOfTable> &v) {
            v.erase(v.begin() + ind);
        });
    }

    void EraseSym(char to_del) {
        if (ended_) return;
        reserved_tape_.reset();
        size_t ind = syms_.find(to_del);
        if (ind == std::string::npos) return;

        std::lock_guard guard(change_table_);
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
        if (ended_ || !CanDoTick()) return;
        if (!reserved_tape_.has_value()) {
            reserved_tape_ = tape_;
        }

        ValueOfTable val = table_[syms_.find(Read(cur_pos_in_tape))][cur_q_];
        if (val.to_write.has_value()) {
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
        if (val.term) {
            ended_ = true;
        }
    }

    bool CorrSym(sf::Uint32 sym) const {
        return syms_.find(sym) != std::string::npos;
    }

    void Reset() {
        if (reserved_tape_.has_value()) {
            tape_ = *reserved_tape_;
        }
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
    std::optional<std::map<int, sf::String>> reserved_tape_;
    mutable std::mutex change_table_;
    bool ended_ = false;

    bool CanDoTick() const {
        return std::any_of(table_.begin(), table_.end(), [] (const std::vector<ValueOfTable> &v) {
            return std::any_of(v.begin(), v.end(), [] (const ValueOfTable &val) {
                return val.term;
            });
        });
    }
};
