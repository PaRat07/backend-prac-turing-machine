#pragma once

#include "bits/stdc++.h"

#include "monomial.h"

class Parser {
public:
    Parser(std::string data)
        : data_((!data.empty() && data[0] != '-' ? '+' + data : data))
        , to_pares_(data_)
    {}


    Monomial GetMonomial() {
        State state;
        while (true) {
            switch (to_pares_[0]) {
                case '^': {

                    break;
                }
                case '+': {
                    break;
                }
                case '-': {
                    break;
                }
                default: {
                    if (to_pares_[0] >= '0' && to_pares_[0] <= '9') {

                    } else if (to_pares_[0] >= 'a' && to_pares_[0] <= 'z') {

                    } else {
                        throw std::
                    }
                    break;
                }
            }
        }
        return state.cur_ans;
    }

    bool Empty() const {
        return to_pares_.empty();
    }

private:
    std::string data_;
    std::string_view to_pares_;

    enum class Prev {
        LETTER,
        SIGN,
        CARET,
        FACTOR,
        NOTHING
    };

    struct State {
        Prev prev = Prev::NOTHING;
        Monomial cur_ans;
    };
};