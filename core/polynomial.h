#pragma once
#include "bits/stdc++.h"

#include "single_linked_list.h"
#include "monomial.h"
#include "parser.h"

class Polynomial {
public:
    Polynomial(const std::string &s) {
        Parser parser(s);
        while (!parser.Empty()) {
            data_.PushBack(parser.GetMonomial());
        }
        Normalize();
    }

    std::string ToString() const {
        return std::transform_reduce(data_.begin(), data_.end(), std::string(), std::plus<>{}, [] (const Monomial &m) {
            return m.ToString();
        });
    }
private:
    SingleLinkedList<Monomial> data_;


    void Normalize() {
        data_.Sort();
        SingleLinkedList<Monomial> new_data;
        for (const Monomial &m : data_) {
            if (!new_data.Empty() && Mergeable(new_data.Back(), m)) {
                new_data.Back().factor += m.factor;
            } else {
                new_data.PushBack(m);
            }
        }
        data_ = std::move(new_data);
    }
};