#include "../turing_machine.h"

TuringMachine::ValueOfTable::ValueOfTable()
        : to_write(std::nullopt)
        , move(Move::DONT_MOVE)
        , q(std::nullopt)
        , term(false)
{
}

TuringMachine::ValueOfTable::ValueOfTable(std::string_view val) {
    if (val[0] != ',') {
        if (val[0] == '!') {
            term = true;
            to_write = std::nullopt;
            move = Move::DONT_MOVE;
            q = std::nullopt;
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

    if (val[0] != ',') {
        if (val[0] == '!') {
            term = true;
            move = Move::DONT_MOVE;
            q = std::nullopt;
            return;
        }
        move = (val[0] == 'L' ? Move::MOVE_LEFT : Move::MOVE_RIGHT);
        val.remove_prefix(2);
    } else {
        move = Move::DONT_MOVE;
        val.remove_prefix(1);
    }

    if (!val.empty()) {
        if (val[0] == '!') {
            term = true;
            q = std::nullopt;
            return;
        }
        val.remove_prefix(1);
        q = std::stoi(std::string(val));
    } else {
        q = std::nullopt;
    }
    term = false;
}

sf::String TuringMachine::GetFrom(int pos, int len) {
    sf::String ans;
    for (int i = pos; i < pos + len; ++i) {
        ans[i - pos] = Read(i);
    }
    return ans;
}

void TuringMachine::Write(int pos, sf::Uint32 sym) {
    ended_ = false;
    reserved_tape_.reset();
    if (sym == lambda) {
        tape_.erase(pos);
    } else {
        tape_[pos] = sym;
    }
}

void TuringMachine::SetValue(sf::Vector2i pos, sf::String s) {
    ValueOfTable val;
    try {
        val = ValueOfTable(std::string(s));
    } catch (...) {
        return;
    }
    if (val.q >= qs_.size()) return;

    ended_ = false;
    reserved_tape_.reset();
    table_[pos.x][pos.y] = val;
}

void TuringMachine::AddLine() {
    ended_ = false;
    reserved_tape_.reset();
    qs_.insert(++last_q_);
    std::lock_guard guard(change_table_);
}

void TuringMachine::AddColumn(sf::Uint32 sym) {
    ended_ = false;
    reserved_tape_.reset();
    syms_;
}

sf::String TuringMachine::GetValue(sf::Vector2i pos) const {
    if  (table_.count(*syms_.find_by_order(pos.x)) && table_.at(*syms_.find_by_order(pos.x)).contains(*qs_.find_by_order(pos.y))) {
        return table_.at(*syms_.find_by_order(pos.x)).at(*qs_.find_by_order(pos.y)).ToStr();
    } else {
        return ValueOfTable().ToStr();
    }
}

sf::Vector2i TuringMachine::Size() const {
    return {(int)syms_.size(), (int)qs_.size()};
}

void TuringMachine::EraseLine(int pos) {
    ended_ = false;
    reserved_tape_.reset();
    auto it = qs_.find_by_order(pos);
    std::lock_guard guard(change_table_);
    std::for_each(table_.begin(), table_.end(), [it] (std::pair<const sf::Uint32, std::map<int, ValueOfTable>> &i) {
        i.second.erase(*it);
    });
    qs_.erase(it);
}

void TuringMachine::EraseColumn(int pos) {
    if (ended_) return;
    reserved_tape_.reset();
    auto it = syms_.begin();
    std::advance(it, pos);
    std::lock_guard guard(change_table_);
    table_.erase(*it);
    syms_.erase(std::next(syms_.begin(), pos));
}

sf::Uint32 TuringMachine::Read(int pos) {
    return (tape_.count(pos) ? tape_[pos] : lambda);
}

void TuringMachine::SetCallBacks(std::function<void()> cb_move_r, std::function<void()> cb_move_l) {
    cb_move_l_ = std::move(cb_move_l);
    cb_move_r_ = std::move(cb_move_r);
}

void TuringMachine::Do1Tick() {
    if (ended_ || !CanDoTick()) return;
    if (!reserved_tape_.has_value()) {
        reserved_tape_ = tape_;
    }

    ValueOfTable val = table_[Read(cur_pos_in_tape)][cur_q_];
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

    if (val.q.has_value()) {
        cur_q_ = *val.q;
    }
    if (val.term) {
        ended_ = true;
    }
}

bool TuringMachine::CorrSym(sf::Uint32 sym) const {
    return syms_.find(sym) != syms_.end();
}

void TuringMachine::Reset() {
    if (reserved_tape_.has_value()) {
        tape_ = *reserved_tape_;
    }
}

bool TuringMachine::CanDoTick() const {
    return std::any_of(table_.begin(), table_.end(), [] (const std::pair<sf::Uint32, std::map<int, ValueOfTable>> &v) {
        return std::any_of(v.second.begin(), v.second.end(), [] (const std::pair<int,  ValueOfTable> &val) {
            return val.second.term;
        });
    });
}

sf::String TuringMachine::GetColumnName(int pos) const {
    return *syms_.find_by_order(pos);
}

sf::String TuringMachine::GetLineName(int pos) const {
    return "q" + std::to_string(*qs_.find_by_order(pos));
}
